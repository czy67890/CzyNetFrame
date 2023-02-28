#include <iostream>
#include "base/LstTimer.h"
#include <mutex>
#include <thread>
#include <memory>
#include <map>
#include <assert.h>
#include <vector>
#include <filesystem>
#include <future>
#include <sys/types.h>
#include <chrono>
#include <stdio.h>
#include "base/BlockQueue.h"
#include "base/BoundedBlockQueue.h"
#include "base/ThreadPool.h"
#include "base/TimeStamp.h"
#include <algorithm>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "base/NonCopyAble.h"
#include "base/CurrentThread.h"
#include "base/AsyncLog.h"
#include "base/Log.h"

using namespace std;
class SharedPtrCopyOnWrite{
public:
    SharedPtrCopyOnWrite(){
        someDataTobeShared = std::make_shared<vector<int>> (2,3);
    }

    void reader(){
        std::shared_ptr<vector<int>> localPtr;
        {
            LockGraund lk(m_mux);
            localPtr = someDataTobeShared;
        }
        ///some read op
        for(auto i:*localPtr){

        }
    }

    void write(){
        LockGraund lk(m_mux);
        if(!someDataTobeShared.unique()){
            std::shared_ptr<vector<int>> newData(new vector<int>(*someDataTobeShared));
            someDataTobeShared.swap(newData);
        }
        assert(someDataTobeShared.unique());
        ///some write op
        (*someDataTobeShared)[0] = 3;
    }
private:
    std::shared_ptr<vector<int>> someDataTobeShared;
    std::mutex m_mux;
};

void threadFunc(){
   while(true){
   }
}
template<typename T>
struct Handle{
Handle(T *p)
    :m_p(p)
{

}
~Handle(){
    delete m_p;
}

T& operator*(){
    return *m_p;
}

T *m_p;
};

class NonCopy :public NonCopyAble{
public:
NonCopy() = default;
NonCopy(const NonCopy &b)
{

}

};


int main(){
    NonCopy a;
    NonCopy b = a;
    return 0;
}

