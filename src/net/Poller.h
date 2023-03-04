#pragma once
#include "src/base/NonCopyAble.h"
#include <vector>
#include <unordered_map>
namespace CzyNetFrame{

class Channel;
class EventLoop;
class TimeStamp;
using ChannelList = std::vector<Channel*>;

class Poller :public NonCopyAble{
public:
    Poller(EventLoop *loop);
    virtual ~Poller() = default;

    virtual TimeStamp poll(int timeOutMs,ChannelList& list) = 0;
    
    bool hasChannel(Channel *channel) const;

    virtual void updateChannel(Channel *channel) = 0;

    virtual void removeChannel(Channel * channel) = 0;

protected:
    EventLoop *m_ownerLoop;
    using ChannelMap = std::unordered_map<int,Channel*>;
    std::unordered_map<int,Channel*> m_mapFdToChannel;
};


}