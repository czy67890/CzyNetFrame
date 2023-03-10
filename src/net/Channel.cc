#include "src/net/Channel.h"
#include "src/net/EventLoop.h"
#include "src/base/Log.h"
#include <poll.h>
#include "Channel.h"
using namespace CzyNetFrame;

const int Channel::KNoneEvent = 0;
const int Channel::KReadEvent = POLLIN|POLLPRI;
const int Channel::KWriteEvent = POLLOUT;


Channel::Channel(EventLoop *loop,int fd)
    :m_ownerLoop(loop),m_fd(fd),m_events(0)
    ,m_revents(0),m_idxs(-1)
{

}

CzyNetFrame::Channel::~Channel()
{

}

/// @brief mainLoop 调用的核心
void CzyNetFrame::Channel::handleEvent()
{   
    ///POLLNVAL代表一个文件打不开的情况
    if(m_revents & POLLNVAL){
        LOG_WARN<<"POLL NVAL ERROR in fd: "<<m_fd;
    }
    else if(m_revents & (POLLERR | POLLERR)){
        if(m_errCb){
            m_errCb;
        }
    }
    else if(m_revents & (POLLIN)){
         m_readCb();
    }
    else if(m_revents & POLLOUT){
        if(m_writeCb){
            m_writeCb;
        }
    }

}

void CzyNetFrame::Channel::remove()
{
    assert(isNoneEvent());
    m_addedToLoop = false;
    m_ownerLoop->removeChannel(this);
}



void CzyNetFrame::Channel::update()
{
    m_ownerLoop->updateChannel(this);
}
