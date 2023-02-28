#pragma once
#include <functional>
namespace CzyNetFrame{



using ErrFuncType = std::function<void()>;
using ReadFuncType = ErrFuncType;
using WriteFuncType = ErrFuncType;

class Channel{
public:


    void handleEvent();
private:
    int m_idxs;
    int m_fd; 
    ErrFuncType m_errCb;
    ReadFuncType m_readCb;
    WriteFuncType m_writeCb;
};

}