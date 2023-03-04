#pragma once
#include "src/base/NonCopyAble.h"
#include <vector>
#include "src/base/CurrentThread.h"
#include "src/base/Log.h"
#include <memory>


namespace CzyNetFrame{


class Poller;
class Channel;
using ChannelList = std::vector<Channel*>;


class EventLoop :public NonCopyAble{
public:
    EventLoop();
    ~EventLoop();

    void loop();


    void updateChannel(Channel * channel);

    bool isInLoopThread() const;

    void assertInLoopThread() ;

    void setPoller(Poller *poller);

private:
    pid_t m_theradId;
    ChannelList m_channelList;
    bool m_isRunning{false};
    std::unique_ptr<Poller> m_uPtrPoller;
};

}