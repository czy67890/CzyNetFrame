#pragma once
#include "src/base/NonCopyAble.h"
#include <functional>
#include <memory>
#include <vector>
#include <string>
using std::string;
namespace CzyNetFrame{
class EventLoop;
class EventLoopThread;


class EventLoopThreadPool:public NonCopyAble {
public:
    using ThreadInitCb = std::function<void(EventLoop *)>;
    EventLoopThreadPool(EventLoop *baseLoop ,const string &nameArg);
    ~EventLoopThreadPool();
    
    void setThreadNum(int threadNum) {m_threadNum = threadNum;}

    void start(const ThreadInitCb &cb =ThreadInitCb());

    EventLoop *getNextLoop();

    std::vector<EventLoop *> getAllLoops();

    bool started() const{
        return m_started;
    }

    const string &getName() const{
        return m_name;
    }

private:
    int m_threadNum{0};
    string m_name;
    EventLoop *m_baseLoop;
    int m_nextIdx{0};
    bool m_started{false};
    std::vector<std::unique_ptr<EventLoopThread>> m_thread;
    std::vector<EventLoop *> m_vecLoops;
};





}
