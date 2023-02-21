#pragma once
#include <mutex>
template< typename T>
class Singleton{
public:
    T &getInstance(){
        std::call_once(once,&Singleton::initSingleton);
        return *t;
    }

    T *getInstance(){
        std::call_once(once,&Singleton::initSingleton);
        return t;
    }

    static void initSingleton(){
        t = new T();
    }    

private:
    static std::once_flag once;
    static T* t;
};


template<typename T>
std::once_flag Singleton<T>::once;
template<typename T>
T *Singleton<T>::t = NULL;



