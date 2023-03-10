#include "net/EventLoop.h"
#include "net/EpollPoller.h"
#include "net/Channel.h"
#include "net/TimerId.h"
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <iostream>
#include <fcntl.h>
#include "base/AsyncLog.h"
#include "base/Log.h"
using namespace CzyNetFrame;
#define BUF_SIZE 1024
char recvBuf[BUF_SIZE] = {0};
class Server{
public:
    Server(int port = 12345);
    ~Server(){}
    int recv(int fd);
    int accept(EventLoop *loop);
    int listenFd() const{return m_listenFd;}
    void send(int fd);
private:
    struct sockaddr_in m_serverAddr;
    int m_listenFd;
    fd_set m_masterSet;
    fd_set m_workingSet;
    int m_maxListen;
};

int Server::accept(EventLoop *loop) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int newFd = ::accept(m_listenFd,(struct sockaddr *)&clientAddr,&clientAddrLen);
    assert(newFd >= 0);
    Channel *newChannel = new Channel(loop,newFd);
    auto func = std::bind(&Server::recv,this,newFd);
    auto funcSend = std::bind(&Server::send,this,newFd);
    newChannel->setWriteCb(funcSend);
    newChannel->enableRead();
    newChannel->setReadCb(func);
    return newFd;
}

void Server::send(int fd)
{   
    ::send(fd,recvBuf,strlen(recvBuf),0);
    LOG_INFO<<"besend";
    bzero(recvBuf,BUF_SIZE);
}

int Server::recv(int fd) {
    ::recv(fd,recvBuf,BUF_SIZE,0);
    LOG_INFO<<"be recvd";
    return 0;
}


Server::Server(int port)
{
    bzero(&m_serverAddr,sizeof(m_serverAddr));
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(port);
    m_serverAddr.sin_addr.s_addr = htons(INADDR_ANY);
    m_listenFd = socket(AF_INET,SOCK_STREAM,0);
    if(m_listenFd < 0){
        std::cout<<"error"<<std::endl;
        exit(1);
    }
    int opt = 1;
    ///设置端口复用
    setsockopt(m_listenFd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    int ret = 0;
    ///将端口与Fd绑定
    ret = bind(m_listenFd,(struct sockaddr *)&m_serverAddr,sizeof(m_serverAddr));
    assert(ret >= 0);
    ///设置侦听以及连接队列的大小
    ret = listen(m_listenFd,5);
    assert(ret >= 0);
}

void Mprintf(){
    LOG_INFO<<"LOg sucess";
}

int main(){
    AsyncLog log("myproc",1024);
    Log::setLogLevel(Log::DEBUG);
    Log::setOutPutFunc(std::bind(&AsyncLog::append,&log,std::placeholders::_1,std::placeholders::_2));
    LOG_INFO<<"info ";
    //Server ser(12345);
    EventLoop *loop = new EventLoop();
    auto threadFunc = [loop](){
        loop->runEvery(3.0,Mprintf);
    };
    std::thread thread(threadFunc);
    loop->loop(); 
    return 0;
}



