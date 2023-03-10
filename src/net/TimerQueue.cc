#include "src/net/TimerQueue.h"
#include <sys/timerfd.h>
#include <unistd.h>
#include "src/net/EventLoop.h"
#include "src/net/Timer.h"
#include "src/net/TimerId.h"
#include <limits>
#include <time.h>

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

template <typename T>
void bZero(T &ptr){
    ::bzero(&ptr,sizeof(T));
}




namespace CzyNetFrame{

int createTimerFd(){

    int timerFd = ::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);
    if(timerFd < 0){
        LOG_SYSFATAL<<"failed to create timer ";
    }
    return timerFd;
}

struct timespec howMuchTimeFromNow(TimeStamp when){
    int64_t microseconds =   when.microSecondsSinceEpoch() - TimeStamp::now().microSecondsSinceEpoch();
    if(microseconds < 100){
        microseconds = 100;
    }    
    struct timespec ts;
    bzero(&ts,sizeof(ts));
    ts.tv_sec = static_cast<time_t> (microseconds/KPerSecondMicroSecondsCount);
    ts.tv_nsec = static_cast<long>((microseconds%KPerSecondMicroSecondsCount) * 1000);
    return ts;
}


/// @brief 
/// @param timerFd 
/// @param now
/// 读取时间文件描述符上的内容 
void readTimerfd(int timerFd,TimeStamp now){
    uint64_t howmany;
    ssize_t n = ::read(timerFd,&howmany,sizeof(howmany));
    if(n != sizeof(howmany)){
        LOG_ERROR << "TimerQueue::handleRead() reads "<<n<<"bytes instead of 8"; 
    }
}

void resetTimerFd(int timerFd,TimeStamp expirations)
{
    struct itimerspec newValue;
    struct itimerspec oldValue;
    bZero(newValue);
    bZero(oldValue);
    newValue.it_value = howMuchTimeFromNow(expirations);
    int ret = ::timerfd_settime(timerFd,0,&newValue,&oldValue);
    if(ret){
        LOG_SYSERR<<"timerfd_settime()";
    }
}




TimerQueue::TimerQueue(EventLoop *loop)
    :m_timerFd(createTimerFd())
    ,m_loop(loop),
    m_timerFdChannel(loop,m_timerFd),
    m_timeList(),
    m_callingExpiredTimer(false)
{
    m_timerFdChannel.setReadCb(std::bind(&TimerQueue::handleRead,this));
    m_timerFdChannel.enableRead();
}

TimerQueue::~TimerQueue()
{
    m_timerFdChannel.disableAll();
    m_timerFdChannel.remove();
    ::close(m_timerFd);
    ///真正持有并且管理timer的是m_timeList
    for(const Entry &timer : m_timeList){
        delete timer.second;
    }
}
TimerId TimerQueue::addTimer(TimerCallBack cb, TimeStamp when, double ineterVal)
{
    Timer *timer = new Timer(std::move(cb),when,ineterVal);
    m_loop->runInLoop(std::bind(&TimerQueue::addTimerInLoop,this,timer));
    return TimerId(timer,timer->sequence());
}
void TimerQueue::cancel(TimerId timerId)
{
    m_loop->runInLoop(std::bind(&TimerQueue::cancelInLoop,this,timerId));
}

void TimerQueue::addTimerInLoop(Timer *timer)
{
    m_loop->assertInLoopThread();
    bool earlistChanged = insert(timer);
    if(earlistChanged){
        resetTimerFd(m_timerFd,timer->expiration());
    }
}

void TimerQueue::cancelInLoop(TimerId timerId)
{
    m_loop->assertInLoopThread();
    assert(m_timeList.size() == m_activeTimers.size());
    ActiveTimer timer(timerId.timer(),timerId.sequence());
    auto iter = m_activeTimers.find(timer);
    if(iter != m_activeTimers.end()){
        size_t n = m_timeList.erase(Entry(iter->first->expiration(),iter->first));
        assert(n == 1);
        delete iter->first;
        m_activeTimers.erase(iter);
        (void)n;
    }
    else if(m_callingExpiredTimer){
        m_cancelingTimer.insert(timer);
    }
    assert(m_timeList.size() == m_activeTimers.size());

}


void TimerQueue::handleRead()
{
    m_loop->assertInLoopThread();
    TimeStamp now(TimeStamp::now());
    readTimerfd(m_timerFd,now);
    std::vector<Entry> expired = getExpired(now);
    m_callingExpiredTimer = true;
    for(const Entry &it : expired){
        it.second->run();
    }
    m_callingExpiredTimer = false;
    reset(expired,now);
}
std::vector<Entry> TimerQueue::getExpired(TimeStamp now)
{
    std::vector<Entry> vecExpired;
    Entry entry = std::make_pair(now,(Timer *)UINTPTR_MAX);
    auto iterEnd = m_timeList.lower_bound(entry);
    ////attention of UINTPTR_MAX
    ////it is the base of lower_bound use
    assert(iterEnd == m_timeList.end() || now < iterEnd->first);
    std::copy(m_timeList.begin(),iterEnd,back_inserter(vecExpired));
    m_timeList.erase(m_timeList.begin(),iterEnd);
    for(const Entry & entry :vecExpired){
        ActiveTimer timer(entry.second,entry.second->sequence());
        size_t n = m_activeTimers.erase(timer);
        assert(n == 1);
        (void)n;
    }
    assert(m_activeTimers.size() == m_timeList.size());
    return vecExpired;
}


/// @brief 将超时但是需要重复的定时器事件重新插入到set中
/// @param expired 
/// @param now 
void TimerQueue::reset(const std::vector<Entry> &expired, TimeStamp now)
{
    TimeStamp nextExpired;
    for(const Entry & entry : expired){
        ActiveTimer timer(entry.second,entry.second->sequence());
        if(entry.second->repeat() && m_cancelingTimer.find(timer) == m_cancelingTimer.end()){
            entry.second->restart(now);
            insert(entry.second);
        }
        else{
            delete entry.second;
        }
    }

    if(!m_timeList.empty()){
        nextExpired = m_timeList.begin()->first;
    }
    if(nextExpired.valid()){
        resetTimerFd(m_timerFd,nextExpired);
    }
}


bool TimerQueue::insert(Timer *timer)
{
    m_loop->assertInLoopThread();
    assert(m_timeList.size() == m_activeTimers.size());
    bool earliestChanged = false;
    TimeStamp when = timer->expiration();
    auto iter = m_timeList.begin();
    if(iter == m_timeList.end() || when < iter->first){
        earliestChanged = true;
    }
    {
        auto res = m_timeList.insert({when,timer});
        assert(res.second);
    }
    {
        auto res = m_activeTimers.insert({timer,timer->sequence()});
        assert(res.second);
    }
    assert(m_timeList.size() == m_activeTimers.size());
    return earliestChanged;
}

}