#pragma once
#include "src/base/NonCopyAble.h"
#include <vector>
#include "src/base/CurrentThread.h"
#include "src/base/Log.h"
#include <memory>
#include <functional>
#include <mutex>
namespace{
    using FUNC = std::function<void()>;    
}

namespace CzyNetFrame{

class Poller;
class Channel;
class TimerId;
class TimerQueue;


using ChannelList = std::vector<Channel*>;


class EventLoop :public NonCopyAble{
public:
    EventLoop();
    ~EventLoop();

    void loop();

    TimerId runAt(TimeStamp stamp,FUNC cb);
    
    TimerId runAfter(double interVal,FUNC cb);

    TimerId runEvery(double interval,FUNC cb);


    void cancel(TimerId toBeCancel);

    void queueInLoop(FUNC cb);

    void runInLoop(FUNC cb);

    void updateChannel(Channel * channel);

    bool isInLoopThread() const;

    void assertInLoopThread() ;

    void setPoller(Poller *poller);

    void removeChannel(Channel *channel);
    
    bool isInLoopThread();

    void doPendingFunc();

    void quit();

private:
    void wakeUp();

    void handleRead();
private:
    int m_wakeUpFd;
    std::unique_ptr<Channel> m_wakeUpChannel;
    pid_t m_theradId;
    ChannelList m_channelList;
    bool m_isRunning{false};
    std::unique_ptr<Poller> m_uPtrPoller;
    Channel *m_currentActive{NULL};
    bool m_isHandleing{false};
    bool m_callingPendingFunctors{false};
    std::mutex m_mux;
    std::vector<FUNC> m_vecPendingFunctor;
    std::unique_ptr<TimerQueue> m_timeQueue;
};
}
