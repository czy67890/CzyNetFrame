#include "LstTimer.h"
void TimerManagerList::deleteNode(TimerNode *nodeToBeDelete) {
    deleteNodeNoDel(nodeToBeDelete);
    delete nodeToBeDelete;
}

void TimerManagerList::addNode(TimerNode *addedNode) {
    if(addedNode == nullptr){
        return;
    }
    TimerNode *nodeInsertPre = m_ptrHead;
    while(nodeInsertPre->m_ptrNodeNext != m_ptrTail) {
        if (nodeInsertPre->m_expireTime >= addedNode->m_expireTime) {
            break;
        }
        nodeInsertPre = nodeInsertPre->m_ptrNodeNext;
    }
    TimerNode *nextNode = nodeInsertPre->m_ptrNodeNext;
    addedNode->m_ptrNodeNext = nextNode;
    nodeInsertPre->m_ptrNodeNext = addedNode;
    addedNode->m_ptrNodePrev = nodeInsertPre;
    nextNode->m_ptrNodePrev = addedNode;
}

void TimerManagerList::adjustNode(TimerNode *adjustNode) {
    deleteNodeNoDel(adjustNode);
    addNode(adjustNode);
}

void TimerManagerList::deleteNodeNoDel(TimerNode *nodeToBeDelete) {
    if(isEmpty()){
        return;
    }
    TimerNode *prev = nodeToBeDelete->m_ptrNodePrev;
    TimerNode *next = nodeToBeDelete->m_ptrNodeNext;
    prev->m_ptrNodeNext = next;
    next->m_ptrNodePrev = prev;
}

void TimerManagerList::tick() {
    if(isEmpty()){
        return;
    }
    ///get sys cur time
    time_t curTime = time(NULL);
    TimerNode *curNode = m_ptrHead->m_ptrNodeNext;
    while(curNode != m_ptrTail){
        if(curNode -> m_expireTime >  curTime) {
            break;
        }
        std::shared_ptr<ClientData> sPtr = curNode->m_wPtrClientData.lock();
        if(sPtr){
            curNode->m_cbFunc(sPtr.get());
        }
        TimerNode *nextNode = curNode->m_ptrNodeNext;
        deleteNode(curNode);
        curNode = nextNode;
    }
}

bool TimerManagerList::isEmpty() {
    return m_ptrHead->m_ptrNodeNext == m_ptrTail;
}




TimerManagerWheel::~TimerManagerWheel() {
    for(int i = 0;i < TIME_SLOTS;++i){
        TimerNode *tmp = m_timeSlots[i];
        while(tmp){
            TimerNode *store = tmp->m_ptrNodeNext;
            delete tmp;
            tmp = store;
        }
        m_timeSlots[i] = NULL;
    }
}

TimerNode *TimerManagerWheel::addNode(int timeout) {

    if(timeout < 0 ){
        return NULL;
    }
    int ticks = timeout / TIME_GAP;
    if(timeout < TIME_GAP){
        ticks = 1;
    }
    ///在转多少圈后删除
    int rot = ticks/TIME_SLOTS;
    int ts = (m_curSlot + (ticks % TIME_SLOTS)) % TIME_SLOTS;
    TimerNode *newTimer = new TimerNode(timeout);
    newTimer->m_rotation = rot;
    newTimer->m_timeSlot = ts;
    TimerNode *head = m_timeSlots[ts];
    TimerNode *headNext = head->m_ptrNodeNext;

    head->m_ptrNodeNext = newTimer;
    newTimer->m_ptrNodePrev = head;
    newTimer->m_ptrNodeNext = headNext;
    if(headNext){
        headNext->m_ptrNodePrev = newTimer;
    }
    return newTimer;
}

TimerManagerWheel::TimerManagerWheel()
    :m_curSlot(0)
{
    for(int i = 0;i < TIME_SLOTS;++i){
        ///初始化头节点
        m_timeSlots[i] = new TimerNode(0);
    }
}

void TimerManagerWheel::delNode(TimerNode *node) {
    int rot = node->m_rotation;
    int time_slot = node->m_timeSlot;
    TimerNode *next = node->m_ptrNodeNext;
    node->m_ptrNodePrev->m_ptrNodeNext = next;
    if(next){
        next->m_ptrNodePrev = node->m_ptrNodePrev;
    }

}

void TimerManagerWheel::tick() {
    TimerNode *nowNode = m_timeSlots[m_curSlot]->m_ptrNodeNext;
    while(nowNode){
        TimerNode *storeNode = nowNode->m_ptrNodeNext;
        nowNode->m_rotation--;
        if(nowNode->m_rotation <= 0){
            std::shared_ptr<ClientData> client = nowNode->m_wPtrClientData.lock();
            if(client){
                nowNode->m_cbFunc(client.get());
            }
            delNode(nowNode);
        }
        nowNode = storeNode;
    }
    m_curSlot = (m_curSlot+1)%TIME_SLOTS;
}