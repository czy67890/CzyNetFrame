#include "src/net/Sockets.h"
#include "src/net/SocketOps.h"
#include "src/net/InetAddress.h"
#include <stdio.h>  
#include"src/base/Log.h"
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
using namespace CzyNetFrame;


CzyNetFrame::Socket::~Socket()
{
    sockets::close(m_sockFd);
}

bool CzyNetFrame::Socket::getTcpInfo(tcp_info *info) const
{
    socklen_t len = sizeof(*info);
    bzero(info,len);
    return  ::getsockopt(m_sockFd,SOL_TCP,TCP_INFO,info,&len);
}

bool CzyNetFrame::Socket::getTcpInfoString(char *buf, int len) const
{
    struct tcp_info tcpi;
  bool ok = getTcpInfo(&tcpi);
  if (ok)
  {
    snprintf(buf, len, "unrecovered=%u "
             "rto=%u ato=%u snd_mss=%u rcv_mss=%u "
             "lost=%u retrans=%u rtt=%u rttvar=%u "
             "sshthresh=%u cwnd=%u total_retrans=%u",
             tcpi.tcpi_retransmits,  // Number of unrecovered [RTO] timeouts
             tcpi.tcpi_rto,          // Retransmit timeout in usec
             tcpi.tcpi_ato,          // Predicted tick of soft clock in usec
             tcpi.tcpi_snd_mss,
             tcpi.tcpi_rcv_mss,
             tcpi.tcpi_lost,         // Lost packets
             tcpi.tcpi_retrans,      // Retransmitted packets out
             tcpi.tcpi_rtt,          // Smoothed round trip time in usec
             tcpi.tcpi_rttvar,       // Medium deviation
             tcpi.tcpi_snd_ssthresh,
             tcpi.tcpi_snd_cwnd,
             tcpi.tcpi_total_retrans);  // Total retransmits for entire connection
  }
  return ok;
}

void CzyNetFrame::Socket::bindAddress(const InetAddress &addr)
{
    sockets::bindOrDie(m_sockFd,addr.getSockAddr());
}

void CzyNetFrame::Socket::listen()
{
    sockets::listenOrDie(m_sockFd);
}   

int CzyNetFrame::Socket::accept(InetAddress *peerAddr)
{
    struct sockaddr_in6 addr;
    bzero(&addr,sizeof(addr));
    int connfd = sockets::accept(m_sockFd,&addr);
    if(connfd >= 0){
        peerAddr->setSockAddrInet6(addr);
    }
    return connfd;
}

void CzyNetFrame::Socket::shutDownWrite()
{
    sockets::shutdownWrite(m_sockFd);
}

void CzyNetFrame::Socket::setTcpNoDelay(bool on)
{
    int optval = on ? 1 : 0;
  ::setsockopt(m_sockFd, IPPROTO_TCP, TCP_NODELAY,
               &optval, static_cast<socklen_t>(sizeof optval));
}

void CzyNetFrame::Socket::setReuseAddr(bool on)
{
     int optval = on ? 1 : 0;
  ::setsockopt(m_sockFd, SOL_SOCKET, SO_REUSEADDR,
               &optval, static_cast<socklen_t>(sizeof optval));
}

void CzyNetFrame::Socket::setReusePort(bool on)
{
    #ifdef SO_REUSEPORT
  int optval = on ? 1 : 0;
  int ret = ::setsockopt(m_sockFd, SOL_SOCKET, SO_REUSEPORT,
                         &optval, static_cast<socklen_t>(sizeof optval));
  if (ret < 0 && on)
  {
    LOG_SYSERR << "SO_REUSEPORT failed.";
  }
#else
  if (on)
  {
    LOG_ERROR << "SO_REUSEPORT is not supported.";
  }
#endif
}

void CzyNetFrame::Socket::setKeepAlive(bool on)
{
    int optval = on ? 1 : 0;
  ::setsockopt(m_sockFd, SOL_SOCKET, SO_KEEPALIVE,
               &optval, static_cast<socklen_t>(sizeof optval));
  // FIXME CHECK
}
