#pragma once
#include <sys/time.h>
#include <stdint.h>
#include <string>
namespace CzyNetFrame{

constexpr int KPerSecondMicroSecondsCount = 1000*1000;

class TimeStamp{
public:
    TimeStamp()
    {

    }

    explicit TimeStamp(const int64_t microSecond)
        :m_microSecondsFromEpoch(microSecond)
    {

    }

    std::string toString() const;

    std::string toFormatedString(bool showMicroSeconds) const; 

    void swap(TimeStamp &rhs){
        std::swap(rhs.m_microSecondsFromEpoch,m_microSecondsFromEpoch);
    }

    static TimeStamp now();

    inline int64_t microSecondsFromEpoch() const{
        return m_microSecondsFromEpoch;
    }
private:
    int64_t m_microSecondsFromEpoch{0};
};
}


