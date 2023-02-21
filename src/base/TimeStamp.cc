#include "src/base/TimeStamp.h"
#include "TimeStamp.h"
std::string CzyNetFrame::TimeStamp::toString() const
{   
    char buf[32] = {0};
    int64_t seconds = m_microSecondsFromEpoch/KPerSecondMicroSecondsCount;
    int64_t microSeconds = m_microSecondsFromEpoch%KPerSecondMicroSecondsCount;
    snprintf(buf,32,"%"PRI64 "%"PRI64"",seconds,microSeconds);
}

std::string CzyNetFrame::TimeStamp::toFormatedString(bool showMicroSeconds) const
{
    char buf[64] = {0};
    time_t seconds = static_cast<time_t>(m_microSecondsFromEpoch/KPerSecondMicroSecondsCount);
    struct tm tm_time;
    gmtime_r(&seconds,&tm_time);
    if(showMicroSeconds){
        int microSecond = static_cast<int> (m_microSecondsFromEpoch % KPerSecondMicroSecondsCount);
        snprintf(buf,sizeof(buf),"%4d%02d%02d %02d:%02d:%02d.%6d",tm_time.tm_year + 1900,tm_time.tm_mon + 1,tm_time.tm_mday,tm_time.tm_hour,tm_time.tm_min,tm_time.tm_sec,microSecond);
    }
    else{
         snprintf(buf,sizeof(buf),"%4d%02d%02d %02d:%02d:%02d",tm_time.tm_year + 1900,tm_time.tm_mon + 1,tm_time.tm_mday,tm_time.tm_hour,tm_time.tm_min,tm_time.tm_sec);
    }
    return buf;
}

TimeStamp CzyNetFrame::TimeStamp::now()
{   
    struct timeval tv;
    int nowSeconds = gettimeofday(&tv,NULL);
    int64_t seconds = tv.tv_sec;
    return TimeStamp(seconds * KPerSecondMicroSecondsCount);
}
