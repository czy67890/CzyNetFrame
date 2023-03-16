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
#include <sys/uio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include "net/TcpConnection.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <sys/un.h>
#include "net/TcpServer.h"
#include "net/InetAddress.h"
#include <fcntl.h>
#include <iostream>
#include <fcntl.h>
#include "base/AsyncLog.h"
#include <signal.h>
#include "base/Log.h"

using namespace CzyNetFrame;
#define BUF_SIZE 1024
char recvBuf[BUF_SIZE] = {0};

class Server {
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
    ret = listen(m_listenFd, 5);
    assert(ret >= 0);
}

void Mprintf() {
    LOG_INFO << "LOg sucess";
}

void daemon_init() {
    int i;
    pid_t pid;
    ///第一次调用fork并exit，目的是让child1成为进程组的组长
    if (pid = fork(); pid < 0) {
        exit(0);
    }
    if (pid > 0) {
        exit(0);
    }

    ///setsid成为会话的组长，屏蔽终端，但此时仍然可以重新打开终端
    setsid();
    ///消除下次调用产生的SIGHUP信号
    signal(SIGHUP, SIG_IGN);
    ///第二次fork将打开终端的功能完全关闭
    if (pid = fork();pid > 0) {
        exit(0);
    }
    ///关闭文件描述符
    for (int i = 0; i < 64; ++i) {
        ::close(i);
    }
    ///重新打开文件描述符
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);
    return;
}

class Te {
public:
    void pruint(int num) {
        std::cout << "test  " << num << std::endl;
    }
};


using memFunc = std::function<void(Te *, int)>;

class TestShared : public std::enable_shared_from_this<TestShared> {
public:
    TestShared() {

    }

    void toBeBinded() {
        std::cout << "bind suc" << std::endl;
        return;
    }

    void testBind() {
        std::function<void()> fuc = std::bind(&TestShared::toBeBinded, shared_from_this());
        fuc();
    }

};


class TestProtected {
public:
    void printProtected() {
        std::cout << "protected called propertyly" << std::endl;
    }
};

class TestPublic {
public:
    void printPublic() {
        std::cout << "public called properly" << std::endl;
    }

};

class AllSub :
        public TestPublic, protected TestProtected {

};

void onlyPrint() {
    LOG_INFO << "i be called properly";
}

class A {

};

class B : public A {

};


void msgcallback(const TcpConnectionPtr &connectionPtr, Buffer *buffer, TimeStamp) {
    LOG_INFO << "recv" << buffer->retrieveAllAsString();
    char *msg = "i be ";
    connectionPtr->send(msg, strlen(msg));
}

int main() {


    /*EventLoop *loop = new EventLoop();
    auto threadFunc = [loop](){
        loop->runEvery(3.0,Mprintf);
    };
    int asn = getservbyname()
    std::thread thread(threadFunc);
    loop->loop(); */
    /*daemon_init();

    LOG_INFO << "info ";
    LOG_INFO << "pid = " << ::getpid();


    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        LOG_INFO << "i be called";
    }*/
    A *a = new A;
    B *b = static_cast<B *>(a);
    AsyncLog log("myproc", 1024 * 1024);
    Log::setLogLevel(Log::DEBUG);
    Log::setOutPutFunc(std::bind(&AsyncLog::append, &log, std::placeholders::_1, std::placeholders::_2));
    EventLoop *mainLoop = new EventLoop();
    InetAddress localAddr(12345);
    TcpServer server(mainLoop, localAddr, "testServer", CzyNetFrame::TcpServer::KReusePort);
    server.setMessageCallback(msgcallback);
    server.setThreadNum(1);
    server.start();
    mainLoop->loop();
    return 0;
}