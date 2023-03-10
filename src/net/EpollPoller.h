#pragma once
#include "src/net/Poller.h"
#include <vector>
#include <sys/epoll.h>
namespace CzyNetFrame{
class TimeStamp;
class EventLoop;
class EpollPoller :public Poller
{
public:
    EpollPoller(EventLoop *loop);


    ~EpollPoller(){


    }

    
    void updateChannel(Channel *channel) override;

    void removeChannel(Channel * channel) override;
    
    TimeStamp poll(int timeOutMs,ChannelList& list) override;



private:
    void update(int op,Channel * channel);
    void fillActiveList(int numEvents,ChannelList &list);
    using EventVec = std::vector<struct epoll_event> ;
    EventVec m_vecEvents;
    
    int m_epollfd;
};



}