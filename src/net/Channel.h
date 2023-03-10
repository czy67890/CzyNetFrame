#pragma once
#include <functional>
namespace CzyNetFrame{


class EventLoop;

using ErrFuncType = std::function<void()>;
using ReadFuncType = ErrFuncType;
using WriteFuncType = ErrFuncType;


class Channel{
public:
    Channel(EventLoop *loop,int fdArg);
    
    ~Channel();
    
    void handleEvent();


    EventLoop *getLoop()
    {
        return m_ownerLoop;
    }

    int fd() const{
        return m_fd;
    }

    int idxs() const{
        return m_idxs;
    }

    void setIdxs(int index){
        m_idxs = index;
    }

    bool isNoneEvent() const{
        return m_events == KNoneEvent;
    }


    int event() const{
        return m_events;
    }

    void enableRead(){
        m_events |= KReadEvent;update();
    }

    void enableWrite(){
        m_events |= KWriteEvent;update();
        update();
    }

    void disableRead(){
        m_events &= ~KReadEvent;update();
    }

    void disableWrite(){
        m_events &= ~KWriteEvent;update();
    }

    void disableAll(){
        m_events = KNoneEvent;
        update();
    }

    void setRevent(int event){
        m_revents = event; 
    }

    void setReadCb(ReadFuncType readF){
        m_readCb = std::move(readF);
    }


    void setWriteCb(WriteFuncType writeF){
        m_writeCb = std::move(writeF);
    }

    void remove();

private:
    void update();
    int m_events;
    int m_revents;
    int m_idxs;
    ErrFuncType m_errCb;
    ReadFuncType m_readCb;
    WriteFuncType m_writeCb;
    const int m_fd;
    EventLoop *m_ownerLoop;
    static const int KNoneEvent;
    static const int KReadEvent;
    static const int KWriteEvent;
    bool m_addedToLoop{true};
};

}