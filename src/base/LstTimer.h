#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>
#include <cstring>
#include <functional>
#include <csignal>
#include <iostream>
#include <vector>
#ifndef TESTSERVER_LSTTIMER_H
#define TESTSERVER_LSTTIMER_H
namespace {
    constexpr int BUF_SIZE = 1024;
}
namespace {
    ///时间槽数量
    constexpr int TIME_SLOTS = 60;
    ///时间间隔
    constexpr int TIME_GAP = 1;
}

class TimerNode;

class TimerManagerWheel{
public:
    TimerManagerWheel();

    ~TimerManagerWheel();


    TimerNode *addNode(int timeout);

    void delNode(TimerNode *node);


    void tick();
private:
    TimerNode *m_timeSlots[TIME_SLOTS];
    int m_curSlot;
};



class ClientData{
public:
    explicit ClientData(int clientFd,const struct sockaddr_in * addr = NULL)
                : clientFd(clientFd)
                {
                    memset(sendBuf,'\0',BUF_SIZE);
                    memset(recvBuf,'\0',BUF_SIZE);
                    if(addr){
                        clientAddr = *addr;
                    }
                }
                ~ClientData(){
                    ::close(clientFd);
                }
    void setTimer(TimerNode* sPtrTime) {
        m_sPtrTimer = sPtrTime;
    }
public:
    struct sockaddr_in clientAddr;
    int clientFd;
    TimerNode* m_sPtrTimer;
    char sendBuf[BUF_SIZE];
    char recvBuf[BUF_SIZE];
};



class TimerNode{
public:
    friend class TimerManagerList;
    friend class TimerManagerWheel;

    TimerNode(time_t expireTime,TimerNode *prev = NULL,TimerNode *next = NULL)
        :m_expireTime(expireTime),m_ptrNodePrev(prev),m_ptrNodeNext(next),m_cbFunc(&TimerNode::defaultCb)
    {
        setCbFunc(defaultCb);
    }

    void setCbFunc(std::function<void (ClientData *)> func){
        m_cbFunc = func;
    }


    void bindClientData(std::shared_ptr<ClientData> clientData){
        m_wPtrClientData = clientData;
    }

private:
    static void defaultCb(ClientData * data){
        return;
    }
    time_t m_expireTime;
    TimerNode *m_ptrNodeNext;
    TimerNode *m_ptrNodePrev;
    std::function<void (ClientData *)> m_cbFunc;
    std::weak_ptr<ClientData> m_wPtrClientData;
    int m_rotation{0};
    int m_timeSlot{0};
};


class TimerManagerList{
public:
    TimerManagerList()
        :m_ptrHead(new TimerNode(0)),m_ptrTail(new TimerNode(0))
    {
        m_ptrHead->m_ptrNodeNext = m_ptrTail;
        m_ptrTail->m_ptrNodePrev = m_ptrHead;
    }

    ~TimerManagerList(){
        while(m_ptrHead->m_ptrNodeNext != m_ptrTail){
            deleteNode(m_ptrHead->m_ptrNodeNext);
        }
        ///不要有裸new和裸delete的操作
        delete m_ptrHead;
        delete m_ptrTail;
    }

    void tick();

    void deleteNode(TimerNode *nodeToBeDelete);

    void addNode(TimerNode *addNode);

    void adjustNode(TimerNode *adjustNode);


private:
    inline bool isEmpty();
    void deleteNodeNoDel(TimerNode *nodeToBeDelete);
private:
    TimerNode *m_ptrHead{NULL};
    TimerNode *m_ptrTail{NULL};
};




#endif //TESTSERVER_LSTTIMER_H
