#pragma once
#include <set>
#include <vector>
#include <functional>
#include <utility>

#include "src/base/NonCopyAble.h"
#include "src/base/TimeStamp.h"
#include "src/net/Channel.h"

using TimerCallBack = std::function<void()>; 


namespace CzyNetFrame{
class EventLoop;
class Timer;
class TimerId;
using Entry = std::pair<TimeStamp,Timer*>;
using TimerList = std::set<Entry>;
using ActiveTimer = std::pair<Timer*,int64_t>;
using ActiveTimerSet = std::set<ActiveTimer>;



class TimerQueue :public NonCopyAble
{
public:
    explicit TimerQueue(EventLoop *loop);
    ~TimerQueue();

    TimerId addTimer(TimerCallBack cb,TimeStamp when,double ineterVal);

    void cancel(TimerId timerId);

private:
    void addTimerInLoop(Timer *timer);
    void cancelInLoop(TimerId timerId);
    void handleRead();
    std::vector<Entry> getExpired(TimeStamp now);

    void reset(const std::vector<Entry> &expired,TimeStamp now);

    bool insert(Timer *timer);


private:
    EventLoop *m_loop;
    const int m_timerFd;
    Channel m_timerFdChannel;
    TimerList m_timeList;

    ActiveTimerSet m_activeTimers;
    bool m_callingExpiredTimer;
    ActiveTimerSet m_cancelingTimer;
}; 




}