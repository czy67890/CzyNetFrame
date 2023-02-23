#include "src/base/Log.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include "src/base/TimeStamp.h"
#include "src/base/TimeZone.h"
#include "src/base/CurrentThread.h"


namespace CzyNetFrame{
thread_local char t_errnoBuf[512];
thread_local char t_time[64];
thread_local time_t t_lastSecond;

const char *strerror_tl(int savedErrno){
    ///将错误信息放入到线程自带的buffer中
    return strerror_r(savedErrno,t_errnoBuf,sizeof(t_errnoBuf));
}

Log::LOG_LEVEL initLogLevel(){
    if(::getenv("CZY_LOG_TRACE")){
        return Log::TRACE;
    }
    else if(::getenv("CZY_LOG_DEBUG")){
        return Log::DEBUG;
    }
    else{
        return Log::INFO;        
    }
}

Log::LOG_LEVEL g_logLevel = initLogLevel();


const char * LogLevelName[Log::NUM_LOG_LEVELS]{
 "TRACE ",
  "DEBUG ",
  "INFO  ",
  "WARN  ",
  "ERROR ",
  "FATAL ",
};



class T{
public:
    T(const char *str,unsigned int len)
        :m_str(str),m_len(len)
    {
        assert(strlen(str) == len);
    }
    const char *m_str;
    const unsigned m_len;
};

inline LogStream &operator<<(LogStream & stream,T v){
    stream.append(v.m_str,v.m_len);
    return stream;
}

inline LogStream& operator<<(LogStream& s, const Log::SourceFile& v)
{
  s.append(v.m_data, v.m_size);
  return s;
}

void defaultOutput(const char* msg, int len)
{
  size_t n = fwrite(msg, 1, len, stdout);
  //FIXME check n
  (void)n;
}

void defaultFlush()
{
  fflush(stdout);
}

Log::OutPutFunc g_output = defaultOutput;
Log::FlushFunc g_flush = defaultFlush;

TimeZone g_timeZone;

}


using namespace CzyNetFrame;

Log::Impl::Impl(Level level,int old_errno,const SourceFile &file,int line)
    :m_time(TimeStamp::now()),m_stream(),m_level(level),m_line(line),m_baseName(file)
{
    formatTime();
    NowThread::tid();
    m_stream<<T(NowThread::tidString(),NowThread::tidStringLen());
    m_stream<<T(LogLevelName[level],6);
    if(old_errno != 0){
        m_stream<<strerror_tl(old_errno)<<"(errno = "<<old_errno<<")";
    }
}


void Log::Impl::formatTime(){
    int64_t microSecondsSinceEpoch = m_time.microSecondsFromEpoch();
  time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / KPerSecondMicroSecondsCount);
  int microseconds = static_cast<int>(microSecondsSinceEpoch % KPerSecondMicroSecondsCount);
  if (seconds != t_lastSecond)
  {
    t_lastSecond = seconds;
    struct DateTime dt;
    if (g_timeZone.valid())
    {
      dt = g_timeZone.toLocalTime(seconds);
    }
    else
    {
      dt = TimeZone::toUtcTime(seconds);
    }

    int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
        dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);
    assert(len == 17); (void)len;
  }

  if (g_timeZone.valid())
  {
    Fmt us(".%06d ", microseconds);
    assert(us.length() == 8);
    m_stream << T(t_time, 17) << T(us.data(), 8);
  }
  else
  {
    Fmt us(".%06dZ ", microseconds);
    assert(us.length() == 9);
    m_stream << T(t_time, 17) << T(us.data(), 9);
  }
}

void Log::Impl::finish(){
    m_stream<<" - "<<m_baseName<<":"<<m_line<<'\n';
}

Log::Log(SourceFile file, int line)
  : m_impl(INFO, 0, file, line)
{
}

Log::Log(SourceFile file, int line, LOG_LEVEL level, const char* func)
  : m_impl(level, 0, file, line)
{
  m_impl.m_stream << func << ' ';
}

Log::Log(SourceFile file, int line, LOG_LEVEL level)
  : m_impl(level, 0, file, line)
{
}

Log::Log(SourceFile file, int line, bool toAbort)
  :m_impl(toAbort?FATAL:ERROR, errno, file, line)
{

}

Log::~Log(){
    ///将文件名和换行符输出
    m_impl.finish();
    const LogStream::Buffer& buf(stream().buffer());
    g_output(buf.data(),buf.length());
    if(m_impl.m_level == FATAL){
        g_flush();
        abort();
    }
}

void Log::setLogLevel(Log::LOG_LEVEL level)
{
  g_logLevel = level;
}

void Log::setOutPutFunc(OutPutFunc func)
{
  g_output = func;
}

void Log::setFlushFunc(FlushFunc flush)
{
  g_flush = flush;
}

void Log::setTimeZone(const TimeZone& tz)
{
  g_timeZone = tz;
}

















