#include "src/net/Poller.h"
#include "src/net/EventLoop.h"
#include "src/net/Channel.h"
#include <unistd.h>
#include <sys/eventfd.h>
#include "src/net/TimerQueue.h"
#include "src/net/TimerId.h"
#include "src/net/EpollPoller.h"
#include "EventLoop.h"
#include "src/net/Timer.h"
using namespace CzyNetFrame;
namespace{

thread_local CzyNetFrame::EventLoop *t_thisThreadOwnALoop = NULL;

int createEventFd(){
    int evFd = ::eventfd(0,EFD_CLOEXEC|EFD_NONBLOCK);
    if(evFd < 0){
        LOG_SYSERR<<"Failed in eventfd";
        abort();
    }
    return evFd;
}


}
CzyNetFrame::EventLoop::EventLoop()
    :m_wakeUpFd(createEventFd()),
    m_wakeUpChannel(new Channel(this,m_wakeUpFd)),
    m_currentActive(NULL),
    m_uPtrPoller(new EpollPoller(this)),
    m_timeQueue(new TimerQueue(this))
{
    if (t_thisThreadOwnALoop)
    {
        LOG_FATAL << " another loop is existed in thi thread\n";
    }
    else
    {
        t_thisThreadOwnALoop = this;
    }

    m_theradId = NowThread::tid();
    m_wakeUpChannel->setReadCb(std::bind(&EventLoop::handleRead,this));
    m_wakeUpChannel->enableRead();
}

CzyNetFrame::EventLoop::~EventLoop()
{
    t_thisThreadOwnALoop = NULL;
}

void CzyNetFrame::EventLoop::loop()
{
    /// 在第一次调用loop前必须非运行状态
    assert(!m_isRunning);
    assertInLoopThread();
    m_isRunning = true;
    
    while (m_isRunning)
    {
        m_channelList.clear();
        m_uPtrPoller->poll(500, m_channelList);
        m_isHandleing = true;
        for (auto &channel : m_channelList)
        {   
            m_currentActive = channel;
            channel->handleEvent();
        }
        m_isHandleing = false;
        doPendingFunc();
    }
}

TimerId CzyNetFrame::EventLoop::runAt(TimeStamp stamp, FUNC cb)
{
    return m_timeQueue->addTimer(std::move(cb),stamp,0.0);
}

TimerId CzyNetFrame::EventLoop::runAfter(double interVal, FUNC cb)
{    
    return m_timeQueue->addTimer(std::move(cb),addTime(TimeStamp::now(),interVal) ,0.0);
}

TimerId CzyNetFrame::EventLoop::runEvery(double interval, FUNC cb)
{
     return m_timeQueue->addTimer(std::move(cb),addTime(TimeStamp::now(),interval) ,interval);
}

void CzyNetFrame::EventLoop::cancel(TimerId toBeCancel)
{
    m_timeQueue->cancel(toBeCancel);
}

void CzyNetFrame::EventLoop::queueInLoop(FUNC cb)
{  
   {
    std::unique_lock<std::mutex > lk(m_mux); 
    m_vecPendingFunctor.push_back(std::move(cb));
   }
   if(!isInLoopThread() || m_callingPendingFunctors){
     wakeUp();
   }
}

void CzyNetFrame::EventLoop::runInLoop(FUNC cb)
{
    if(isInLoopThread()){
        cb();
    }
    else{
        queueInLoop(cb);
    }
}

void CzyNetFrame::EventLoop::updateChannel(Channel *channel)
{
    m_uPtrPoller->updateChannel(channel);
}

bool CzyNetFrame::EventLoop::isInLoopThread() const
{
    return NowThread::tid() == m_theradId;
}

void CzyNetFrame::EventLoop::assertInLoopThread()
{
    if (!isInLoopThread())
    {
        LOG_FATAL << "eventloop not run in his owner thread \n";
    }
}

void CzyNetFrame::EventLoop::setPoller(Poller *poller)
{
    m_uPtrPoller.reset(poller);
}

void CzyNetFrame::EventLoop::removeChannel(Channel *channel)
{
    assert(channel->getLoop() == this);
    assertInLoopThread();
    if(m_isHandleing){
        assert(m_currentActive == channel || std::find(m_channelList.begin(),m_channelList.end(),channel) == m_channelList.end());
    }
    m_uPtrPoller->removeChannel(channel);
}

void CzyNetFrame::EventLoop::doPendingFunc()
{
    std::vector<FUNC> functors;
    m_callingPendingFunctors = true;
    {
        std::unique_lock<std::mutex> lk(m_mux);
        functors.swap(m_vecPendingFunctor);
    }
    for(const auto &func:functors){
        func();
    }
    m_callingPendingFunctors = false;
}


void CzyNetFrame::EventLoop::wakeUp()
{
   uint64_t one = 1;
   size_t n = ::write(m_wakeUpFd,&one,sizeof(one));
   if(n != sizeof(one)){
    LOG_ERROR<<"Event loop wakeup failed";
   }
}


void CzyNetFrame::EventLoop::handleRead()
{
    uint64_t one = 1;
    size_t n = ::read(m_wakeUpFd,&one,sizeof(one));
    if(n != sizeof(one)){
        LOG_ERROR << "handleRead failed";
    }
}