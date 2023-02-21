#pragma once
#include "src/base/NonCopyAble.h" 
#include <mutex>
#include <queue>
#include <condition_variable>
using LockGraund = std::unique_lock<std::mutex>;
template<typename T>
class BlockQueue :public NonCopyAble{
public:
    BlockQueue()
    {

    }   
    void push(const T& t){
        std::unique_lock<std::mutex> lk(m_mux);
        m_blockQueue.push(t);
        m_condVar.notify_one();
    }

    T pop(){
        std::unique_lock<std::mutex> lk(m_mux);
        while(m_blockQueue.empty()){
            m_condVar.wait(lk);
        }
        assert(!m_blockQueue.empty());
        T front(std::move(m_blockQueue.front()));
        m_blockQueue.pop();
        return front;
    }

private:
    std::queue<T> m_blockQueue;
    std::mutex m_mux;
    std::condition_variable m_condVar;
};

