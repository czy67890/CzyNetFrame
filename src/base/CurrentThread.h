#pragma once
#include <thread>
#include <string>
namespace CzyNetFrame{
namespace NowThread{


extern thread_local int t_cachedTid ;
extern thread_local char t_tidString[32];
extern thread_local int t_tidStrLen;
extern thread_local const char *t_threadName;

void cacheTid();

inline int tid(){
    ///使用分支预测来提高运行速度
    if(__builtin_expect(t_cachedTid == 0,0)){
        cacheTid();
    }
    return t_cachedTid;
}

inline const char *tidString(){
    return t_tidString;
}


inline int tidStringLen(){
    return t_tidStrLen;
}

inline const char *name(){
    return t_threadName;
}

bool isMainThread();


std::string stackTrace(bool demangle);



}


}