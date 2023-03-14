#pragma once
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "src/base/BoundedBlockQueue.h"
#include "src/base/LogStream.h"
#include <string>
#include <functional>
#include <thread>
#include "src/base/Log.h"
#include "src/base/Singleton.h"
using std::string;


namespace CzyNetFrame{

class AsyncLog{
public:
    AsyncLog(const string baseName,off_t rollSize,int flushIntervals = 3);

    ~AsyncLog(){
        if(m_isRunning){
            stop();
        }
    }

    void append(const char *logLine,int len);

    void start(){
        m_isRunning = true;
        m_thread = std::thread{std::bind(&AsyncLog::threadFunc,this)};
        
    }

    void stop(){
        m_isRunning = false;
        m_cond.notify_all();
        m_thread.join();
    }

private:
    void threadFunc();

    using Buffer = CzyNetFrame::FixedBuffer<CzyNetFrame::KLargeBuffer>; 
    using BufferVector = std::vector<std::unique_ptr<Buffer>>;
    using BufferPtr = BufferVector::value_type;

    const int m_flushInterval;
    std::atomic<bool> m_isRunning{true};
    const string m_baseName;
    const off_t m_rollSize;
    std::thread m_thread;
    std::mutex m_mux;
    std::condition_variable m_cond;
    BufferPtr m_currentBuffer;
    BufferPtr m_nextBuffer;
    BufferVector m_vecBuffer;
};

}






