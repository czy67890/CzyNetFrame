#pragma once
#include <stdint.h>
namespace CzyNetFrame{

class Timer;

class TimerId
{
public:
    TimerId(Timer *time,int64_t sequence)
        :m_timer(time),
        m_sequence(sequence)
    {
    }


    ~TimerId() = default;

    Timer *timer() const{
        return m_timer;
    }

    int64_t sequence() const{
        return m_sequence;
    }
    
private:
    Timer *m_timer;
    int64_t m_sequence;
};

}