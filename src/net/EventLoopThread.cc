#include "src/net/EventLoopThread.h"
#include "src/net/EventLoop.h"
using namespace CzyNetFrame;

CzyNetFrame::EventLoopThread::EventLoopThread()
    : m_isExisting(false)
{
}

CzyNetFrame::EventLoopThread::~EventLoopThread()
{
    m_isExisting = true;
    if(m_loop == NULL){
        m_loop->quit();
        m_loopThread.join();
    }
}

EventLoop *CzyNetFrame::EventLoopThread::startLoop()
{
    m_loopThread = std::thread(std::bind(&EventLoopThread::threadFunc, this));
    {
        std::unique_lock<std::mutex> lk(m_mux);
        while (m_loop == NULL)
        {
            m_cond.wait(lk);
        }
    }
    return m_loop;
}

void CzyNetFrame::EventLoopThread::threadFunc()
{
    EventLoop loop;

    if (m_cb)
    {
        m_cb(&loop);
    }

    {
        std::unique_lock<std::mutex> lk(m_mux);
        m_loop = &loop;
        m_cond.notify_one();
    }

    m_loop->loop();
    std::unique_lock<std::mutex> lk(m_mux);
    m_loop = NULL;
}