#include "src/net/EpollPoller.h"
#include <cassert>
#include <cstring>
#include "src/net/Channel.h"
#include "src/base/Log.h"
using namespace CzyNetFrame;
namespace
{
    constexpr int KNEW = -1;
    constexpr int KDEL = 2;
    constexpr int KADDED = 3;
    constexpr int KInitEventSize = 16;
}

EpollPoller::EpollPoller(EventLoop *loop)
    : Poller(loop),m_vecEvents(KInitEventSize)
{
    m_epollfd = epoll_create(1);
}

void CzyNetFrame::EpollPoller::updateChannel(Channel *channel)
{
    int idx = channel->idxs();
    int fd = channel->fd();
    if (idx == KNEW || idx == KDEL)
    {
        /// in the case of new FD;
        if (idx == KNEW)
        {
            assert(m_mapFdToChannel.find(fd) == m_mapFdToChannel.end());
            m_mapFdToChannel[fd] = channel;
        }
        else
        {
            assert(m_mapFdToChannel.find(fd) != m_mapFdToChannel.end());
            assert(m_mapFdToChannel[fd] == channel);
        }
        channel->setIdxs(KADDED);
        update(EPOLL_CTL_ADD, channel);
    }
    else
    {
        assert(m_mapFdToChannel.find(fd) != m_mapFdToChannel.end());
        assert(m_mapFdToChannel[fd] == channel);
        assert(idx == KADDED);
        if (channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL, channel);
            channel->setIdxs(KDEL);
        }
        else
        {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}


///remove Channel主要的工作是使得EPOLL_WAIT不再关注该事件
void CzyNetFrame::EpollPoller::removeChannel(Channel *channel)
{   
    int fd = channel->fd();
    auto iter = m_mapFdToChannel.find(fd);
    assert(iter != m_mapFdToChannel.end());
    assert(iter->second == channel);
    int index = channel->idxs();
    assert(index != KDEL);
    m_mapFdToChannel.erase(iter);
    if(index == KADDED){
        update(EPOLL_CTL_DEL,channel);
    }
    channel->setIdxs(KDEL);
}

TimeStamp CzyNetFrame::EpollPoller::poll(int timeOutMs, ChannelList &list)
{
    int savedErrno = errno;
    TimeStamp now (TimeStamp::now());
    int numEvents = ::epoll_wait(m_epollfd, &(*m_vecEvents.begin()), m_vecEvents.size(), timeOutMs);
    if (numEvents > 0)
    {
        fillActiveList(numEvents,list);   
        if(numEvents == m_vecEvents.size()){
            m_vecEvents.resize(2 * m_vecEvents.size() );
        }
    }
    else if(numEvents == 0){
        LOG_TRACE<<"nothing happend yet";
    }
    else{
        if(savedErrno != EINTR){
            errno = savedErrno;
            LOG_SYSERR<<"Epoll_wait failure ";
        }
    }
    return now;
}

void CzyNetFrame::EpollPoller::update(int op, Channel *channel)
{
    struct epoll_event event;
    bzero(&event, sizeof(event));
    event.events = channel->event();
    int fd = channel->fd();
    event.data.ptr = channel;
    if (::epoll_ctl(m_epollfd, op, fd, &event) < 0)
    {
        LOG_SYSERR << "epoll_ctl failed with " << fd;
    }
    else
    {
        LOG_INFO << "epoll_ctl success";
    }
}

void CzyNetFrame::EpollPoller::fillActiveList(int numEvent,ChannelList &list)
{   
    assert(static_cast<size_t>(numEvent) <= m_vecEvents.size());
    for(int i = 0;i < numEvent;++i){
        Channel* curChannel = static_cast<Channel*>(m_vecEvents[i].data.ptr);
#ifndef NDEBUG
        ChannelMap::const_iterator iter = m_mapFdToChannel.find(curChannel->fd());
        assert(iter != m_mapFdToChannel.end());
        assert(iter->second == curChannel);
#endif
        curChannel->setRevent(m_vecEvents[i].events);
        list.push_back(curChannel);
    }
}