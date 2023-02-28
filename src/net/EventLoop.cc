#include "src/net/EventLoop.h"
#include "src/net/Poller.h"
#include "src/net/Channel.h"
using namespace CzyNetFrame;

void CzyNetFrame::EventLoop::loop()
{
    ///�ڵ�һ�ε���loopǰ���������״̬
    assert(!m_isRunning);
    assertInLoopThread();
    m_isRunning = true;
    while(m_isRunning){
        m_channelList.clear();
        m_uPtrPoller->poll(m_channelList);
        for(auto &channel:m_channelList){
            channel->handleEvent();
        }
    }
}

bool CzyNetFrame::EventLoop::isInLoopThread() const
{
   return NowThread::tid() == m_theradId;
}

void CzyNetFrame::EventLoop::assertInLoopThread()
{
    if(!isInLoopThread()){
        LOG_FATAL<<"eventloop not run in his owner thread \n";
    }

}
