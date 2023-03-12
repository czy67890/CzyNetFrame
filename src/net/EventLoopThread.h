#pragma once
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>
#include "src/base/NonCopyAble.h"
#include <functional>
using std::string;

namespace CzyNetFrame{

class EventLoop;
using LoopInitCallBack = std::function<void(EventLoop *)>;


class EventLoopThread
    :public NonCopyAble
{
public:
    explicit EventLoopThread(const LoopInitCallBack &cb = LoopInitCallBack(),const string &name = string());

    ~EventLoopThread();

    EventLoop * startLoop();

    void threadFunc();




private:
    std::mutex m_mux;
    std::condition_variable m_cond;
    std::thread m_loopThread;
    EventLoop * m_loop{NULL};
    bool m_isExisting{false};
    LoopInitCallBack m_cb;
};




}