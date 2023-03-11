#pragma once
#include "src/base/NonCopyAble.h"

struct tcp_info;

namespace CzyNetFrame{
class InetAddress;
class Socket{
public:
    explicit Socket(int sockfd)
        :m_sockFd(sockfd)
    {
    }

    ~Socket();

    int fd() const {return m_sockFd;}

    bool getTcpInfo(struct tcp_info * info) const;

    bool getTcpInfoString(char *buf,int len) const;

    void bindAddress(const InetAddress & addr);

    void listen();

    ///value-result 
    int accept(InetAddress *peerAddr);

    void shutDownWrite();

    void setTcpNoDelay(bool on);


    void setReuseAddr(bool on);

    void setReusePort(bool on);


    void setKeepAlive(bool on);

private:
    const int m_sockFd;
};
}

