#pragma once

#include "src/net/Channel.h"
#include "src/net/Sockets.h"
#include "src/base/NonCopyAble.h"
#include <functional>
namespace CzyNetFrame{
class EventLoop;
class InetAddress;
class Acceptor
    :public NonCopyAble
{
public:
typedef std::function<void (int, const InetAddress&)> NewConnectionCallback;
    Acceptor(EventLoop *loop,const InetAddress & listenAddr,bool reusePort);

    ~Acceptor();

    void setNewConnectionCallBack(const NewConnectionCallback& cb);

    void listen();

    bool isListening() const{
        return m_isListen;
    }


private:

    void handleRead();
    
    EventLoop *m_loop;
    Socket m_listenSocket;
    Channel m_readChannel;
    NewConnectionCallback m_acceptCb;
    bool m_isListen;
    int m_idleFd;
};



}
