#include "src/base/LogFile.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "src/base/FileUnit.h"
using namespace CzyNetFrame;
LogFile::LogFile(const string &baseName,
                 off_t rollSize, bool theradSafe,
                 int flushInterval,
                 int checkEveryN)
                 :m_baseName(baseName),
                m_rollSize(rollSize),
                m_flushInterVal(flushInterval),
                m_checkEveryN(checkEveryN),
                m_count(0),
                m_startOfPeriod(0),
                m_lastFlush(0),
                m_lastRoll(0)

{
    assert(baseName.find('/') == string::npos);
    rollFile();
}

CzyNetFrame::LogFile::~LogFile() = default;

void CzyNetFrame::LogFile::flush()
{
    if(m_threadSafe){
        std::unique_lock<std::mutex> lk(m_mutex);
        m_file->flush();
    }
    else{
        m_file->flush();
    }
}


/// @brief append��Ҫʵ����������,��һ�Ǽ�������,�ڶ��������ﵽһ������ļ������д�������ļ�=
/// @param logLine 
/// @param len 
void CzyNetFrame::LogFile::append(const char *logLine, int len)
{
    if(m_threadSafe){
        std::unique_lock<std::mutex> lk(m_mutex);
        appendFileUnlock(logLine,len);
    }
    else{
        appendFileUnlock(logLine,len);
    }
}



bool CzyNetFrame::LogFile::rollFile()
{
    time_t now = 0;
    string fileName = getLogFileName(m_baseName,&now);
    ///��ȡ�����������
    time_t start = now / KRollPerSeconds * KRollPerSeconds;
    if(now > m_lastRoll){
        m_lastRoll = now;
        m_lastFlush = now;
        m_startOfPeriod = start;
        m_file.reset(new AppendFile(fileName));
        return true;
    }
    return false;
}

void CzyNetFrame::LogFile::appendFileUnlock(const char *logLine, int len)
{
    m_file->append(logLine,len);
    if(m_file->writtenBytes() > m_rollSize){
        rollFile();
        return;
    }
    ///���������һ����û��д�κ���־,Ҳ��ˢ��,��Ϊ�첽��־ÿ�������ø�appendһ��
    ++m_count;
    if(m_count >= m_checkEveryN){
        m_count = 0;
        time_t now = ::time(NULL);
        time_t thisP = now / KRollPerSeconds*KRollPerSeconds;
        ///����һ�컻�ļ�
        if(thisP != m_startOfPeriod){
            rollFile();
        }
        else if(now - m_lastFlush > m_flushInterVal){
            m_lastFlush = now;
            flush();
        }
    }
}

string CzyNetFrame::LogFile::getLogFileName(const string &basename, time_t *now)
{
  string filename;
  filename.reserve(basename.size() + 64);
  filename = basename;
  char timebuf[32];
  struct tm tm;
  *now = time(NULL);
  gmtime_r(now, &tm); // FIXME: localtime_r ?
  strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S", &tm);
  filename += timebuf;
  char pidbuf[32];
  snprintf(pidbuf, sizeof pidbuf, ".%d", ::getpid());
  filename += pidbuf;
  filename += ".log";

  return filename;
}
