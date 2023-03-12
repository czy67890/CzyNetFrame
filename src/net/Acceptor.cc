#include "src/net/Acceptor.h"
#include "src/base/Log.h"
#include "src/net/InetAddress.h"
#include "src/net/SocketOps.h"
#include "src/net/EventLoop.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

using namespace CzyNetFrame;

CzyNetFrame::Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr, bool reusePort)
    : m_loop(loop), m_listenSocket(sockets::createNonblockingOrDie(listenAddr.family())),
      m_readChannel(m_loop, m_listenSocket.fd()), m_idleFd(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
    assert(m_idleFd >= 0);
    m_listenSocket.setReusePort(reusePort);
    m_listenSocket.setReuseAddr(true);
    m_listenSocket.bindAddress(listenAddr);
    m_readChannel.setReadCb(std::bind(&Acceptor::handleRead, this));
}

CzyNetFrame::Acceptor::~Acceptor()
{
    m_readChannel.disableAll();
    m_readChannel.remove();
    ::close(m_idleFd);
}

void CzyNetFrame::Acceptor::setNewConnectionCallBack(const NewConnectionCallback &cb)
{
    m_acceptCb = cb;
}

void CzyNetFrame::Acceptor::listen()
{
    m_loop->assertInLoopThread();
    m_isListen = true;
    m_listenSocket.listen();
    m_readChannel.enableRead();
}

void CzyNetFrame::Acceptor::handleRead()
{
    m_loop->assertInLoopThread();
    InetAddress peerAddr;
    int connfd = m_listenSocket.accept(&peerAddr);
    if (connfd >= 0)
    {
        if (m_acceptCb)
        {
            m_acceptCb(connfd, peerAddr);
        }
        else
        {
            sockets::close(connfd);
        }
    }
    else
    {
        LOG_SYSERR << " in acceptor ::handleRead too many connfd";
        ////this tricks to defend two many client
        if (errno == EMFILE)
        {
            ::close(m_idleFd);
            m_idleFd = ::accept(m_listenSocket.fd(), NULL, NULL);
            ::close(m_idleFd);
            m_idleFd = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
        }
    }
}
