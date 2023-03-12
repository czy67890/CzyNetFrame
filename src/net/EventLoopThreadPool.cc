#include "src/net/EventLoopThreadPool.h"
#include "src/net/EventLoop.h"
#include "src/net/EventLoopThread.h"
#include <assert.h>

using namespace CzyNetFrame;
EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseLoop, const string &nameArg)
    :m_baseLoop(baseLoop),m_name(nameArg)
{

}

CzyNetFrame::EventLoopThreadPool::~EventLoopThreadPool()
{

}

void CzyNetFrame::EventLoopThreadPool::start(const ThreadInitCb &cb)
{
    assert(!m_started);
    m_baseLoop->assertInLoopThread();
    m_started = true;
    for(int i = 0;i < m_threadNum;++i){
        char buf[m_name.size() + 32];
        snprintf(buf,sizeof(buf),"%s%d",m_name.c_str(),i);
        EventLoopThread * t =  new EventLoopThread(cb,buf);
        m_thread.push_back(std::unique_ptr<EventLoopThread>(t));
        m_vecLoops.push_back(t->startLoop());
    }
    if(m_threadNum == 0&& cb){
        cb(m_baseLoop);        
    }
}

EventLoop *CzyNetFrame::EventLoopThreadPool::getNextLoop()
{
    m_baseLoop->assertInLoopThread();
    assert(m_started);
    EventLoop *loop = m_baseLoop;
    if(!m_vecLoops.empty()){
        loop = m_vecLoops[m_nextIdx];
        m_nextIdx = (m_nextIdx + 1)%m_threadNum;
    }  
    return loop;
}

std::vector<EventLoop *> CzyNetFrame::EventLoopThreadPool::getAllLoops()
{
    return m_vecLoops;
}
