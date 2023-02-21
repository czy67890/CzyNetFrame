#include <iostream>
#include "base/LstTimer.h"
#include <mutex>
#include <thread>
#include <memory>
#include <map>
#include <assert.h>
#include <vector>
#include <future>
#include <chrono>
#include "base/BlockQueue.h"
#include "base/BoundedBlockQueue.h"
#include "base/ThreadPool.h"
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

int main(){ 
    std::shared_ptr<int> test1(new int(3));
    std::shared_ptr<int> test2(new int(4));
    std::shared_ptr<int> test3 = test1;
    test1.swap(test2);
    auto ali = alignof('c');
    cout<<ali<<endl;
    cout<<*test1<<endl;
    
    cout<<*test2<<endl;
    cout<<*test3<<endl;
    return 0;
}

