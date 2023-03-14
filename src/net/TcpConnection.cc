#include "src/net/TcpConnection.h"
#include "src/base/TimeStamp.h"
#include "src/net/Sockets.h"
#include "src/net/Channel.h"
#include "src/base/Log.h"
#include "src/net/EventLoop.h"
#include "src/net/SocketOps.h"
#include "src/net/WeakCallBack.h"
#include "src/net/TimerId.h"

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

void TcpConnection::forceClose() {
    if (m_stat == ConnectionStat::KCONNECT || m_stat == ConnectionStat::KDISCONNECTING) {
        setState(ConnectionStat::KDISCONNECTING);
        m_loop->queueInLoop(std::bind(&TcpConnection::forceCloseInLoop, shared_from_this()));
    }
}

void CzyNetFrame::TcpConnection::forceCloseWithDelay(double seconds) {
    if (m_stat == ConnectionStat::KCONNECT || m_stat == ConnectionStat::KDISCONNECTING) {
        setState(ConnectionStat::KDISCONNECTING);
        m_loop->runAfter(seconds, makeWkCallBack(shared_from_this(), &TcpConnection::forceClose));
    }
}

void TcpConnection::shutDown() {
    if (m_stat == ConnectionStat::KCONNECT) {
        setState(ConnectionStat::KDISCONNECTING);
        m_loop->runInLoop(std::bind(&TcpConnection::shutdownInLoop, shared_from_this()));

    }

}

void TcpConnection::setTcpNoDelay(bool on) {
    m_socket->setTcpNoDelay(on);
}

void TcpConnection::startRead() {
    m_loop->runInLoop(std::bind(&TcpConnection::startReadInLoop, shared_from_this()));
}

void TcpConnection::stopRead() {
    m_loop->runInLoop(std::bind(&TcpConnection::stopReadInLoop, shared_from_this()));
}

void TcpConnection::connectionEstablished() {
    m_loop->assertInLoopThread();
    assert(m_stat == ConnectionStat::KCONNECTING);
    setState(ConnectionStat::KCONNECT);
    m_channel->tie(shared_from_this());
    m_channel->enableRead();
    m_connectionCb(shared_from_this());
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
            m_loop->queueInLoop(std::bind(m_highWaterCb, shared_from_this(), oldLen + remianing));
        }
        m_outputBuffer.append(static_cast<const char *>(message) + nwroted, remianing);
        if (!m_channel->isWriting()) {
            m_channel->enableWrite();
        }
    }
}

void TcpConnection::shutdownInLoop() {
    m_loop->assertInLoopThread();
    if (m_channel->isWriting()) {
        m_socket->shutDownWrite();
    }
}

void TcpConnection::forceCloseInLoop() {
    m_loop->assertInLoopThread();
    if (m_stat == ConnectionStat::KCONNECT || m_stat == ConnectionStat::KDISCONNECTING) {
        handleClose();
    }
}

const char *TcpConnection::stateToString() const {
    switch (m_stat) {
        case ConnectionStat::KDISCONNECTED:
            return "kDisconnected";
        case ConnectionStat::KCONNECTING:
            return "kConnecting";
        case ConnectionStat::KCONNECT:
            return "kConnected";
        case ConnectionStat::KDISCONNECTING:
            return "kDisconnecting";
        default:
            return "unknown state";
    }
}
}

void TcpConnection::startReadInLoop() {
    m_loop->assertInLoopThread();
    if (!m_reading || !m_channel->isReading()) {
        m_channel->enableRead();
        m_reading = true;
    }
}

void TcpConnection::stopReadInLoop() {
    m_loop->assertInLoopThread();
    if (m_reading || m_channel->isReading()) {
        m_channel->disableRead();
        m_reading = false;
    }

}
