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

#include "base/CurrentThread.h"


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
    cout<<CzyNetFrame::NowThread::t_cachedTid<<endl;  
}


int main(){ 
    int fd = ::open("./a.txt",O_RDWR|O_CLOEXEC);
    std::filesystem::path m_path("./a.txt");
    struct stat statBuf;
    ::fstat(fd,&statBuf);
        int* fileSize = new int();
        if(fileSize){
            ///±£´æÎÄ¼þ×´Ì¬
            if(std::filesystem::is_regular_file(m_path)){
                 int64_t modTime = std::chrono::duration_cast<std::chrono::seconds>(std::filesystem::last_write_time(m_path).time_since_epoch()).count();
                std::time_t cfTime = std::chrono::system_clock::to_time_t( std::chrono::file_clock::to_sys(std::filesystem::last_write_time(m_path)));
                if(cfTime == (std::time_t)(statBuf.st_mtime)){
                    cout<<"correct";
                }
                std::cout<<cfTime<<statBuf.st_mtime;
            }
               
        }

    thread t1(threadFunc);
    t1.join();
    return 0;
}

