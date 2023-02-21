#pragma once
#include <thread>
#include "src/base/BlockQueue.h"
#include <array>
#include <queue>
#include <functional>
#include <condition_variable>
template <size_t N>
class ThreadPool
{
    using  Threads = std::array<std::thread,N>;
public:
    ThreadPool(){
        for(int i = 0;i < N;++i){
            auto func = std::bind(&ThreadPool::runner,this);
            m_threads[i] = std::thread(func);
        }
    }

    ~ThreadPool(){
        if(m_running){
            stop();
        }
    }

    void strat(){
        m_running = true;
    }

    void stop(){
        m_running = false;
        m_condEmpty.notify_all();
        for(int i = 0;i < N;++i){
            m_threads[i].join();
        }
    }

    void pushTask(std::function<void()>);
private:
    void runner();
private:
    std::mutex m_mux;
    std::condition_variable m_condEmpty;
    Threads m_threads;
    std::queue<std::function<void()>> m_taskQueue;
    bool m_running{true};
};

template<size_t N>
void ThreadPool<N>::pushTask(std::function<void()> task){
    m_taskQueue.push(task);
    m_condEmpty.notify_one();
}


template<size_t N>
void ThreadPool<N>::runner()
{   
    std::function<void()> task;
    while(m_running){
        {
            LockGraund lk(m_mux);
            while(m_taskQueue.empty() && m_running){
                m_condEmpty.wait(lk);    
            }
            if(!m_running){
                break;
            }
            task = m_taskQueue.front();
            m_taskQueue.pop();
        }
        task();
    }
}