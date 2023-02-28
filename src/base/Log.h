#pragma once
#include "src/base/LogStream.h"
#include "src/base/TimeZone.h"
#include "src/base/TimeStamp.h"

#include <functional>


namespace CzyNetFrame{
class TimeStamp;
class Log{
public:
    enum LOG_LEVEL{
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };

    class SourceFile{
    public:
    template<size_t N> 
    SourceFile(const char (&arr)[N])
        :m_size(N -1),m_data(arr)
    {   
        char *startChar = strrchr(arr,'/');
        if(startChar){
            m_data = startChar + 1;
            m_size = m_size - (startChar - arr  + 1);
        }
    }

    SourceFile(const char *fileName)
        :m_data(fileName)
    {  
        const char *startChar = strrchr(fileName,'/');
        if(startChar){
            m_data = startChar + 1;
        }
        m_size = strlen(m_data);
    }
    const char * m_data;
    int m_size;
    };
    

    Log(SourceFile file,int line);
    Log(SourceFile file,int line,LOG_LEVEL level);
    Log(SourceFile file,int line,LOG_LEVEL level,const char *func);
    Log(SourceFile file,int line,bool toAbort);
    ~Log();
    LogStream &stream(){
        return m_impl.m_stream;
    }

    static LOG_LEVEL getLogLevel();

    static void setLogLevel(LOG_LEVEL level);


    typedef std::function<void(const char *,int )> OutPutFunc;
    typedef std::function<void()> FlushFunc;

    static void setOutPutFunc(OutPutFunc func);

    static void setFlushFunc(FlushFunc func);


    static void setTimeZone(const TimeZone &timeZone);


private:
    class Impl{
    public:
        typedef Log::LOG_LEVEL Level;
        Impl(Level level,int old_errno,const SourceFile &file,int line);    
        void formatTime();
        void finish();
        TimeStamp m_time;
        LogStream m_stream;
        Level m_level;
        int m_line;
        SourceFile m_baseName;
    };
    Impl m_impl;


};

extern Log::LOG_LEVEL g_logLevel;

inline Log::LOG_LEVEL Log::getLogLevel(){
    return g_logLevel;
}




const char* strerror_tl(int savedErrno);

// Taken from glog/logging.h
//
// Check that the input is non NULL.  This very useful in constructor
// initializer lists.

#define CHECK_NOTNULL(val) \
  ::CzyNetFrame::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(Log::SourceFile file, int line, const char *names, T* ptr)
{
  if (ptr == NULL)
  {
   Log(file, line, Log::FATAL).stream() << names;
  }
  return ptr;
}

}


#define LOG_TRACE if (CzyNetFrame::Log::getLogLevel() <= CzyNetFrame::Log::TRACE) \
  CzyNetFrame::Log(__FILE__, __LINE__, CzyNetFrame::Log::TRACE, __func__).stream()
#define LOG_DEBUG if (CzyNetFrame::Log::getLogLevel() <= CzyNetFrame::Log::DEBUG) \
  CzyNetFrame::Log(__FILE__, __LINE__, CzyNetFrame::Log::DEBUG, __func__).stream()
#define LOG_INFO if (CzyNetFrame::Log::getLogLevel() <= CzyNetFrame::Log::INFO) \
  CzyNetFrame::Log(__FILE__, __LINE__).stream()
#define LOG_WARN CzyNetFrame::Log(__FILE__, __LINE__, CzyNetFrame::Log::WARN).stream()
#define LOG_ERROR CzyNetFrame::Log(__FILE__, __LINE__, CzyNetFrame::Log::ERROR).stream()
#define LOG_FATAL CzyNetFrame::Log(__FILE__, __LINE__, CzyNetFrame::Log::FATAL).stream()
#define LOG_SYSERR CzyNetFrame::Log(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL CzyNetFrame::Log(__FILE__, __LINE__, true).stream()