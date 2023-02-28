#pragma once
#include "src/base/NonCopyAble.h"
#include <vector>
#include "src/base/CurrentThread.h"
#include "src/base/Log.h"
#include <memory>


namespace CzyNetFrame{
class Channel;
class Poller;
using ChannelList = std::vector<Channel*>;


thread_local EventLoop* t_thisThreadOwnALoop{NULL}; 
class EventLoop :public NonCopyAble{
public:
    EventLoop()
    {
        if(t_thisThreadOwnALoop){
            LOG_FATAL<<" another loop is existed in thi thread\n";
        }
        else{

            t_thisThreadOwnALoop = this;
        }

        m_theradId = NowThread::tid();

    }


    ~EventLoop(){
        t_thisThreadOwnALoop = NULL;
    }

    void loop();


    bool isInLoopThread() const;

    void assertInLoopThread() ;

private:
    pid_t m_theradId;
    ChannelList m_channelList;
    bool m_isRunning{false};
    std::unique_ptr<Poller> m_uPtrPoller;
};


}