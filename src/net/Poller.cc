#include "src/net/Channel.h"
#include "src/net/EventLoop.h"
#include "src/net/Poller.h"
#include "src/net/EpollPoller.h"
using namespace CzyNetFrame;
CzyNetFrame::Poller::Poller(EventLoop *loop)
    :m_ownerLoop(loop)
{

}

bool CzyNetFrame::Poller::hasChannel(Channel *channel) const
{
    auto iter = m_mapFdToChannel.find(channel->fd());
    if(iter != m_mapFdToChannel.end()){
        return iter->second == channel;
    }
    return false;
}
