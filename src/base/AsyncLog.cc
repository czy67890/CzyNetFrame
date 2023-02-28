#include "src/base/AsyncLog.h"
#include "src/base/LogFile.h"
#include <chrono>
#include "src/base/TimeStamp.h"


CzyNetFrame::AsyncLog::AsyncLog(const string baseName, off_t rollSize, int flushIntervals)
    :m_baseName(baseName),m_rollSize(rollSize),m_flushInterval(flushIntervals),
    m_currentBuffer(new Buffer),m_nextBuffer(new Buffer)
{
    m_currentBuffer->bzero();
    m_nextBuffer->bzero();
    m_vecBuffer.reserve(16);
    CzyNetFrame::Log::setOutPutFunc(std::bind(&CzyNetFrame::AsyncLog::append,this,std::placeholders::_1,std::placeholders::_2));
    start();
}


////Ç°¶Ë
void CzyNetFrame::AsyncLog::append(const char *logLine,int len)
{
    std::unique_lock<std::mutex> lk(m_mux);
    if(m_currentBuffer->avail() > len){
        m_currentBuffer->append(logLine,len);
    }
    else{
        m_vecBuffer.push_back(std::move(m_currentBuffer));
        
        if(m_nextBuffer){
            m_currentBuffer = std::move(m_nextBuffer);
        }
        else{
            m_currentBuffer.reset(new Buffer());
        }
        m_currentBuffer->append(logLine,len);
    }
    m_cond.notify_one();
}

///  ºó¶Ë
void CzyNetFrame::AsyncLog::threadFunc()
{
    assert(m_isRunning == true);
    LogFile outPut(m_baseName,m_rollSize,false);
    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);
    newBuffer1->bzero();
    newBuffer2->bzero();
    while(true){
        assert(newBuffer1 && newBuffer1->length() == 0);
        assert(newBuffer2 && newBuffer2->length() == 0);
        assert(buffersToWrite.empty());
        {
            std::unique_lock<std::mutex> lk(m_mux);
            if(m_vecBuffer.empty()){
                m_cond.wait_until(lk,std::chrono::system_clock::now() + std::chrono::seconds(m_flushInterval));
            }
            m_vecBuffer.push_back(std::move(m_currentBuffer));
            m_currentBuffer = std::move(newBuffer1);
            buffersToWrite.swap(m_vecBuffer);
            if(!m_nextBuffer){
                m_nextBuffer = std::move(newBuffer2);
            }
        }
        assert(!buffersToWrite.empty());
        if(buffersToWrite.size() > 25){
            char buf[256];
            snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
               TimeStamp::now().toFormatedString().c_str(),
               buffersToWrite.size()-2);
            fputs(buf, stderr);
            outPut.append(buf, static_cast<int>(strlen(buf)));
            buffersToWrite.erase(buffersToWrite.begin()+2, buffersToWrite.end());
        }
        for(const auto & buffer :buffersToWrite){
             outPut.append(buffer->data(),buffer->length());   
        }
        if(buffersToWrite.size() > 2){
            buffersToWrite.resize(2);
        }
        if(!newBuffer1){
            assert(!buffersToWrite.empty());
            newBuffer1 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            ///this reset not the smater pointer reset
            newBuffer1->reset();    
        }
        if(!newBuffer2){
            assert(!buffersToWrite.empty());
            newBuffer2 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer2->reset();
        }
        buffersToWrite.clear();
        outPut.flush();
    }
    outPut.flush();
}
