#pragma once 
#include <memory>
#include <string.h>
#include <mutex>
#include"src/base/NonCopyAble.h"
using std::string;
namespace CzyNetFrame{

class AppendFile;
class LogFile:public NonCopyAble
{
public:
    LogFile(const string & baseName,off_t rollSize,bool theradSafe = true,int flushInterval = 3,int checkEveryN = 1024);    
   
   
    ~LogFile();

    void flush();

    void append(const char *logLine,int len);

    bool rollFile();
private:

    void appendFileUnlock(const char * logLine,int len);

    static string getLogFileName(const string& basename, time_t* now);

private:
    const string m_baseName;
    const off_t m_rollSize;
    const int m_flushInterVal;
    int m_checkEveryN;
    int m_count;
    time_t m_startOfPeriod;
    time_t m_lastRoll;
    time_t m_lastFlush;
    bool m_threadSafe;
    std::mutex m_mutex;
    std::unique_ptr<AppendFile> m_file;
    const static int KRollPerSeconds = 60*60*24;
};




}