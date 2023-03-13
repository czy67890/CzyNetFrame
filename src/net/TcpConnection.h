//
// Created by czy on 2023/3/13.
//

#ifndef CZYSERVER_TCPCONNECTION_H
#define CZYSERVER_TCPCONNECTION_H

#include "src/base/NonCopyAble.h"
#include <string>

using std::string;

#include <memory>
#include <any>
#include <functional>
#include "src/net/InetAddress.h"
#include "src/net/Buffer.h"

struct tcp_info;

namespace CzyNetFrame {

    class TcpConnetion;

    class Buffer;

    class TimeStamp;

    using TcpConnectionPtr = std::shared_ptr<TcpConnetion>;
    using ConnectionCallBack = std::function<void(const TcpConnectionPtr &connectionPtr)>;
    using MessageCallBack = std::function<void(const TcpConnectionPtr &connectionPtr, Buffer, TimeStamp)>;
    using HighWaterCallBack = std::function<void(const TcpConnectionPtr &, size_t)>;

    class EventLoop;

    class Socket;

    class Channel;

    enum class ConnectionStat : short {
        KCONNECT = 0,
        KCONNECTING = 1,
        KDISCONNECTED,
        KDISCONNECTING
    };


    class TcpConnection : public NonCopyAble, std::enable_shared_from_this<TcpConnetion> {

    public:
        TcpConnection(EventLoop *loop, const string &name, int sockfd, const InetAddress &localAddr,
                      const InetAddress &peerAddr);


        ~TcpConnection();

        EventLoop *getLoop() const { return m_loop; }

        const string &name() const { return m_name; }

        const InetAddress &localAddress() const { return m_localAddr; }


        const InetAddress &peerAddress() const { return m_peerAddr; }

        inline bool connected() const { return m_stat == ConnectionStat::KCONNECT; }

        inline bool isDisConnected() const { return m_stat == ConnectionStat::KDISCONNECTED; }

        bool getTcpInfo(struct tcp_info *info);

        void send(const void *message, int len);

        void send(const StringPiece &str);

        void send(Buffer *message);


        void shutDown();

        void forceClose();

        void forceCloseWithDelay(double seconds);


        void setTcpNoDelay(bool on);

        void startRead();


        void stopRead();

        bool isReading() const { return m_reading; }


        void setContext(const std::any &context) {
            m_content = context;
        }

        const std::any &getContext() const {
            return m_content;
        }

        std::any *getMutableContext() {
            return &m_content;
        }

        void setConnectionCb(const ConnectionCallBack &cb) {
            m_connectionCb = cb;
        }

        void setMessageCb(const MessageCallBack &msgCb) {
            m_msgCb = msgCb;
        }

        void setWriteFinishCb(const ConnectionCallBack &cb) {
            m_sendFinishCb = cb;
        }

        void setHighWaterMarkCb(const HighWaterCallBack &cb) {
            m_highWaterCb = cb;
        }

        void setCloseCb(const ConnectionCallBack &closeCb) {
            m_closeCb = closeCb;
        }

        Buffer *inputBuffer() { return &m_inputBuffer; }

        Buffer *outputBuffer() { return &m_outputBuffer; }

        void connectionEstablished();

        void connectionDestory();

    private:
        void handleRead(TimeStamp receiveTime);

        void handleWrite();

        void handleClose();

        void handleError();

        // void sendInLoop(string&& message);
        void sendInLoop(const StringPiece &message);

        void sendInLoop(const void *message, size_t len);

        void shutdownInLoop();

        // void shutdownAndForceCloseInLoop(double seconds);
        void forceCloseInLoop();

        void setState(ConnectionStat s) { m_stat = s; }

        const char *stateToString() const;

        void startReadInLoop();

        void stopReadInLoop();

    private:
        ConnectionStat m_stat;
        EventLoop *m_loop;
        const string m_name;
        bool m_reading;
        std::unique_ptr<Socket> m_socket;
        std::unique_ptr<Channel> m_channel;
        const InetAddress m_localAddr;
        const InetAddress m_peerAddr;
        ConnectionCallBack m_connectionCb;
        ConnectionCallBack m_sendFinishCb;
        ConnectionCallBack m_closeCb;
        MessageCallBack m_msgCb;
        HighWaterCallBack m_highWaterCb;
        size_t m_highWater{64 * 1024 * 1024};
        Buffer m_inputBuffer;
        Buffer m_outputBuffer;
        std::any m_content;
    };
}
#endif //CZYSERVER_TCPCONNECTION_H
