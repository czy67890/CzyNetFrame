#include "src/net/TcpConnection.h"
#include "src/base/TimeStamp.h"
#include "src/net/Sockets.h"
#include "src/net/Channel.h"
#include "src/base/Log.h"
#include "src/net/EventLoop.h"
#include "src/net/SocketOps.h"

using namespace CzyNetFrame;

CzyNetFrame::TcpConnection::TcpConnection(CzyNetFrame::EventLoop *loop, const string &name,
                                          int sockfd,
                                          const CzyNetFrame::InetAddress &localAddr,
                                          const CzyNetFrame::InetAddress &peerAddr)
        : m_loop(loop), m_name(name), m_stat(ConnectionStat::KCONNECTING), m_localAddr(localAddr), m_peerAddr(peerAddr),
          m_socket(new Socket(sockfd)),
          m_channel(new Channel(loop, sockfd)) {
    m_channel->setReadCb(std::bind(&TcpConnection::handleRead, this, std::placeholders::_1));
    m_channel->setWriteCb(std::bind(&TcpConnection::handleWrite, this));
    m_channel->setCloseCb(std::bind(&TcpConnection::handleClose, this));
    m_channel->setErrCb(std::bind(&TcpConnection::handleError, this));
    LOG_INFO << "tcpconnetion::ctor[" << m_name << "]at" << this << "  this " << sockfd;
    ///TODO:this is a long connect default
    ///change it to adapt short connect
    m_socket->setKeepAlive(true);
}

bool CzyNetFrame::TcpConnection::getTcpInfo(struct tcp_info *info) {
    return m_socket->getTcpInfo(info);
}

CzyNetFrame::TcpConnection::~TcpConnection() {

    LOG_DEBUG << "TcpConnection::dtor[" << m_name << "] at " << this
              << " fd=" << m_channel->fd()
              << " state=" << stateToString();
    assert(m_stat == ConnectionStat::KDISCONNECTED);
}


void CzyNetFrame::TcpConnection::send(const void *message, int len) {
    send(StringPiece((char *) message, len));
}

void CzyNetFrame::TcpConnection::send(const CzyNetFrame::StringPiece &str) {
    if (m_stat == ConnectionStat::KCONNECT) {
        if (m_loop->isInLoopThread()) {
            sendInLoop(str);
        } else {
            ///in order to make override clearly
            using memFunc = void (TcpConnection::*)(const StringPiece &str);
            memFunc fp = &TcpConnection::sendInLoop;
            m_loop->runInLoop(std::bind(fp, this, str));
        }
    }
}

void CzyNetFrame::TcpConnection::send(CzyNetFrame::Buffer *message) {
    if (m_stat == ConnectionStat::KCONNECT) {
        if (m_loop->isInLoopThread()) {
            sendInLoop(message->toStringPiece());
            message->retrieveAll();
        } else {
            ///in order to make override clearly
            using memFunc = void (TcpConnection::*)(const StringPiece &str);
            memFunc fp = &TcpConnection::sendInLoop;
            m_loop->runInLoop(std::bind(fp, this, message->retrieveAllAsString()));
        }
    }
}

void CzyNetFrame::TcpConnection::forceClose() {

}

void CzyNetFrame::TcpConnection::forceCloseWithDelay(double seconds) {

}

void TcpConnection::shutDown() {

}

void TcpConnection::setTcpNoDelay(bool on) {

}

void TcpConnection::startRead() {

}

void TcpConnection::stopRead() {

}

void TcpConnection::connectionEstablished() {

}

void TcpConnection::connectionDestory() {

}

void TcpConnection::handleRead(TimeStamp receiveTime) {

}

void TcpConnection::handleWrite() {

}

void TcpConnection::handleClose() {

}

void TcpConnection::handleError() {

}

void TcpConnection::sendInLoop(const StringPiece &message) {
    sendInLoop(message.data(), message.size());
}

void TcpConnection::sendInLoop(const void *message, size_t len) {
    m_loop->assertInLoopThread();
    size_t nwroted = 0;
    size_t remianing = len;
    bool fault = false;
    if (m_stat == ConnectionStat::KDISCONNECTED) {
        LOG_WARN << "disconnected ,give up ";
        return;
    }
    if (!m_channel->isWriting() && m_outputBuffer.readableBytes() == 0) {
        nwroted = sockets::write(m_channel->fd(), message, len);
        if (nwroted >= 0) {
            remianing -= nwroted;
            if (remianing == 0 && m_sendFinishCb) {
                m_loop->queueInLoop(std::bind(m_sendFinishCb, shared_from_this()));
            }
        } else {
            nwroted = 0;
            if (errno != EWOULDBLOCK) {
                if (errno == EPIPE || errno == ECONNRESET) {
                    fault = true;
                }
            }
        }
    }
    assert(remianing <= len);
    if (!fault && remianing > 0) {
        size_t oldLen = m_outputBuffer.readableBytes();
        if (oldLen + remianing >= m_highWater
            && oldLen < m_highWater
            && m_highWaterCb) {
            m_loop->queueInLoop(std::bind(m_highWaterCb, shared_from_this(), oldLen + remaining));
        }
        m_outputBuffer.append(static_cast<const char *>(message) + nwroted, remianing);
        if (!m_channel->isWriting()) {
            m_channel->enableWrite();
        }
    }
}

void TcpConnection::shutdownInLoop() {

}

void TcpConnection::forceCloseInLoop() {

}

const char *TcpConnection::stateToString() const {
    return nullptr;
}

void TcpConnection::startReadInLoop() {

}

void TcpConnection::stopReadInLoop() {

}
