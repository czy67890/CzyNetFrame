#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include "src/base/NonCopyAble.h"
#include <functional>
namespace CzyNetFrame{

using LoopInitCallBack = std::function<void()>;
class EventLoop;

class EventLoopThread
    :public NonCopyAble
{



public:
    explicit EventLoopThread();

    ~EventLoop();

    void startLoop();

    void threadFunc();




private:
    std::mutex m_mux;
    std::condition_variable m_cond;
    std::thread m_loopThread;
    EventLoop * m_loop;
    bool m_isExisting{false};
    LoopInitCallBack m_cb;
};




}