#pragma once
#include "src/base/NonCopyAble.h"
#include <vector>
#include <unordered_map>


namespace CzyNetFrame{

class Channel;
struct pollfd;

class Poller :public NonCopyAble{
public:
    Poller();
    virtual ~Poller();

    void poll(ChannelList & list);
private:
    std::vector<pollfd> m_fds;
    std::unordered_map<int,Channel*> m_mapFdTo
};



}