#pragma once
#include "src/base/NonCopyAble.h" 
#include <mutex>
#include <condition_variable>
#include <array>
#include "src/base/BlockQueue.h"
template<typename T,size_t N>
class BoundedBlockQueue :public NonCopyAble{
public:
    BoundedBlockQueue()
        :m_nowQueueHead(0),m_nowQueueTail(0)
    {

    }   
    void push(const T& t){
        LockGraund lk(m_mux);
        while(isFullNoLock()){
            m_condIsFull.wait(lk);
        }
        assert(!isFullNoLock());
        m_BoundedBlockQueue[m_nowQueueTail] = t;
        m_nowQueueTail = (m_nowQueueTail + 1)%N;
        m_condIsEmpty.notify_one();
    }

    T pop(){
        std::unique_lock<std::mutex> lk(m_mux);
        while(isEmptyNoLock()){
            m_condIsEmpty.wait(lk);
        }
        assert(!isEmptyNoLock());
        T front(std::move(m_BoundedBlockQueue[m_nowQueueHead]));
        m_nowQueueHead = (m_nowQueueHead + 1)%N;
        m_condIsFull.notify_one();
        return front;
    }

    bool isEmpty(){
        LockGraund lk(m_mux);
        return isEmptyNoLock();
    }

    bool isFull(){
        LockGraund lk(m_mux);
        return isFullNoLock();
    }
private:
    inline bool isEmptyNoLock(){
        return m_nowQueueHead == m_nowQueueTail;
    }

    inline bool isFullNoLock(){
        return (m_nowQueueTail + 1)%N == m_nowQueueHead; 
    }

private:
    std::array<T,N> m_BoundedBlockQueue;
    std::mutex m_mux;
    std::condition_variable m_condIsEmpty;
    std::condition_variable m_condIsFull;
    int m_nowQueueHead;
    int m_nowQueueTail;
};
