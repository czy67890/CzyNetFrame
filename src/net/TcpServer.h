//
// Created by czy on 2023/3/15.
//
#include "src/net/TcpConnection.h"
#include <map>
#include <string>
#include "src/base/NonCopyAble.h"
#include <memory>
#include <atomic>
#include "src/net/CallbackDefine.h"

using std::string;
#ifndef CZYSERVER_TCPSERVER_H
#define CZYSERVER_TCPSERVER_H
namespace CzyNetFrame {
    class InetAddress;

    class Acceptor;

    class EventLoopThreadPool;

    class TcpServer : public NonCopyAble {
        enum Option {
            KNoReusePort,
            KReusePort
        };

    public:
        using ConnectionMap = std::map<string, TcpConnectionPtr>;
        using ThreadInitCb = std::function<void(EventLoop *)>;


        TcpServer(EventLoop *loop, const InetAddress &addr, const string &nameArg, Option opt);

        ~TcpServer();

        const string &ipPort() const { return m_ipPort; }

        const string &name() const { return m_name; }

        EventLoop *getLoop() const { return m_loop; }

        void setThreadNum(int threadNum);

        void setThreadInitCallBack(const ThreadInitCb &cb) {
            m_threadInitCb = cb;
        }

        std::shared_ptr<EventLoopThreadPool> threadPool() { return m_eventLoopThreadPool; }

        void start();

        void setConnectionCallback(const ConnectionCallBack &cb) { m_connectionCb = cb; }

        /// Set message callback.
        /// Not thread safe.
        void setMessageCallback(const MessageCallBack &cb) { m_msgCb = cb; }

        /// Set write complete callback.
        /// Not thread safe.
        void setWriteCompleteCallback(const ConnectionCallBack &cb) { m_sendFinishCb = cb; }


    private:
        void newConnection(int sockfd, const InetAddress &peerAddr);

        void removeConnection(const TcpConnectionPtr &conn);

        void removeConnectionInLoop(const TcpConnectionPtr &conn);

    private:
        EventLoop *m_loop;
        const string m_ipPort;
        const string m_name;
        std::unique_ptr<Acceptor> m_acceptor;
        std::shared_ptr<EventLoopThreadPool> m_eventLoopThreadPool;
        ConnectionCallBack m_connectionCb;
        MessageCallBack m_msgCb;
        ConnectionCallBack m_sendFinishCb;
        ThreadInitCb m_threadInitCb;
        std::atomic<int32_t> m_started;
        int m_nextConnId;
        ConnectionMap m_connections;
    };


}


#endif //CZYSERVER_TCPSERVER_H
