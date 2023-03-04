#include "src/net/Poller.h"
#include "src/net/EventLoop.h"
#include "src/net/Channel.h"
#include "EventLoop.h"
using namespace CzyNetFrame;

thread_local CzyNetFrame::EventLoop *t_thisThreadOwnALoop = NULL;
CzyNetFrame::EventLoop::EventLoop()
{
    if (t_thisThreadOwnALoop)
    {
        LOG_FATAL << " another loop is existed in thi thread\n";
    }
    else
    {

        t_thisThreadOwnALoop = this;
    }

    m_theradId = NowThread::tid();
}

CzyNetFrame::EventLoop::~EventLoop()
{
    t_thisThreadOwnALoop = NULL;
}

void CzyNetFrame::EventLoop::loop()
{
    /// 在第一次调用loop前必须非运行状态
    assert(!m_isRunning);
    assertInLoopThread();
    m_isRunning = true;
    while (m_isRunning)
    {
        m_channelList.clear();
        m_uPtrPoller->poll(500, m_channelList);
        for (auto &channel : m_channelList)
        {
            channel->handleEvent();
        }
    }
}

void CzyNetFrame::EventLoop::updateChannel(Channel *channel)
{
    m_uPtrPoller->updateChannel(channel);
}

bool CzyNetFrame::EventLoop::isInLoopThread() const
{
    return NowThread::tid() == m_theradId;
}

void CzyNetFrame::EventLoop::assertInLoopThread()
{
    if (!isInLoopThread())
    {
        LOG_FATAL << "eventloop not run in his owner thread \n";
    }
}

void CzyNetFrame::EventLoop::setPoller(Poller *poller)
{
    m_uPtrPoller.reset(poller);
}