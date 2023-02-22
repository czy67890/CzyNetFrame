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
#include "base/TimeStamp.h"
#include <algorithm>
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
    std::vector<int> vecs{1,2,3,4,5};
    int count = vecs.size();
    std::generate(vecs.begin(),vecs.end(),[&count](){
        return --count;
    });
    for(auto i:vecs){
        std::cout<<i<<endl;
    }
    std::cout<<count<<endl;
    return 0;
}

