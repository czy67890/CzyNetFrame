//
// Created by czy on 2023/3/15.
//
#include "src/net/TcpServer.h"
#include "src/net/Acceptor.h"
#include "src/net/EventLoopThreadPool.h"
#include "src/base/Log.h"
#include "src/net/EventLoop.h"
#include "src/net/SocketOps.h"

using namespace CzyNetFrame;

void defaultConnectionCallBcak(const TcpConnectionPtr &conn) {
    LOG_TRACE << conn->localAddress().toIpPort() << " -> "
              << conn->peerAddress().toIpPort() << " is "
              << (conn->connected() ? "UP" : "DOWN");
}

void defaultMessageCallback(const TcpConnectionPtr &,
                            Buffer *buf,
                            TimeStamp) {
    buf->retrieveAll();
}


TcpServer::TcpServer(EventLoop *loop, const InetAddress &addr, const string &nameArg, TcpServer::Option opt)
        : m_loop(loop), m_acceptor(new Acceptor(m_loop, addr, opt == KReusePort)),
          m_eventLoopThreadPool(new EventLoopThreadPool(loop, nameArg)), m_ipPort(addr.toIpPort()),
          m_connectionCb(defaultConnectionCallBcak), m_msgCb(defaultMessageCallback), m_nextConnId(1), m_started(0) {
    m_acceptor->setNewConnectionCallBack(
            std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer() {
    m_loop->assertInLoopThread();
    LOG_TRACE << "Tcp Server ::[" << m_name << "] is destorying";
    for (auto &item: m_connections) {
        TcpConnectionPtr conn(item.second);
        conn->getLoop()->runInLoop(std::bind(&TcpConnection::connectionDestory, conn));
    }
}

void CzyNetFrame::TcpServer::newConnection(int sockfd, const CzyNetFrame::InetAddress &peerAddr) {
    m_loop->assertInLoopThread();
    EventLoop *ioLoop = m_eventLoopThreadPool->getNextLoop();
    char buf[64];
    snprintf(buf, sizeof buf, "-%s#%d", m_ipPort.c_str(), m_nextConnId);
    ++m_nextConnId;
    string connName = m_name + buf;

    LOG_INFO << "TcpServer::newConnection [" << m_name
             << "] - new connection [" << connName
             << "] from " << peerAddr.toIpPort();
    InetAddress localAddr(sockets::getLocalAddr(sockfd));
    TcpConnectionPtr conn(new TcpConnection(ioLoop, connName,
                                            sockfd,
                                            localAddr,
                                            peerAddr));
    m_connections[connName] = conn;
    conn->setConnectionCb(m_connectionCb);
    conn->setMessageCb(m_msgCb);
    conn->setWriteFinishCb(m_sendFinishCb);
    conn->setCloseCb(
            std::bind(&TcpServer::removeConnection, this, std::placeholders::_1)); // FIXME: unsafe
    ioLoop->runInLoop(std::bind(&TcpConnection::connectionEstablished, conn));

}

void CzyNetFrame::TcpServer::removeConnectionInLoop(const CzyNetFrame::TcpConnectionPtr &conn) {
    m_loop->assertInLoopThread();
    LOG_INFO << "TcpServer::removeConnectionInLoop [" << m_name
             << "] - connection " << conn->name();
    size_t n = m_connections.erase(conn->name());
    (void) n;
    assert(n == 1);
    EventLoop *ioLoop = conn->getLoop();
    ioLoop->queueInLoop(
            std::bind(&TcpConnection::connectionDestory, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr &conn) {
    m_loop->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::start() {
    if (atomic_fetch_add(&m_started, 1) == 0) {
        m_eventLoopThreadPool->start(m_threadInitCb);
        assert(!m_acceptor->isListening());
        m_loop->runInLoop(std::bind(&Acceptor::listen, m_acceptor.get()));
    }
}

void TcpServer::setThreadNum(int threadNum) {
    assert(0 <= threadNum);
    m_eventLoopThreadPool->setThreadNum(threadNum);
}

