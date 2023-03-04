#include "src/net/Timer.h"
using namespace CzyNetFrame;
std::atomic<int64_t> Timer::m_numTimerCrated{0};
void CzyNetFrame::Timer::restart(TimeStamp now)
{
    if(m_repeat){
        m_expiration = addTime(now,m_interval);
    }
    else{
        m_expiration = TimeStamp::invalid();
    }
}