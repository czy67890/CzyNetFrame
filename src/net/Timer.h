#pragma once
#include <atomic>
#include "src/base/TimeStamp.h"
#include "src/base/NonCopyAble.h"
#include <functional>
namespace CzyNetFrame{

using TimeCallBack = std::function<void(void)> ;
class Timer :public NonCopyAble{
public:
    Timer(TimeCallBack cb,TimeStamp when,double interval)
        :m_timeCb(std::move(cb)),m_expiration(when),m_interval(interval),m_repeat(interval > 0.0),
        m_sequence(m_numTimerCrated.fetch_add(1))
    {

    }
    ~Timer(){

    }

    void run(){
        m_timeCb();
    }

    void restart(TimeStamp now);

    TimeStamp expiration() const {
        return m_expiration;
    }

    bool repeat() const {
        return m_repeat;
    }

    static int64_t numCreated(){
        return m_numTimerCrated;
    }

private:
    const TimeCallBack m_timeCb;
    TimeStamp m_expiration;
    const double m_interval;
    const bool m_repeat;
    const int64_t m_sequence;
    static std::atomic<int64_t> m_numTimerCrated;
};
}