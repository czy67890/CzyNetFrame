#include "src/base/FileUnit.h"
#include "src/base/Log.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "FileUnit.h"
#include <iostream>
using namespace CzyNetFrame;
AppendFile::AppendFile(StringArg fileName)
    :m_fp(::fopen(fileName.c_str(),"ae"))
{
    assert(m_fp);
    std::cout<<fileName.c_str()<<std::endl;
    ::setbuffer(m_fp,m_buffer,sizeof(m_buffer));
}

AppendFile::~AppendFile(){
    ::fclose(m_fp);
}

void AppendFile::append(const char *charLine,const size_t size){
    size_t written = 0;
    while(written != size){
        size_t remain = size - written;
        size_t thisTimeWrite = write(charLine + written,remain);
        if(thisTimeWrite != remain){
            //获取当前文件上的错误
            int err = ferror(m_fp);
            if(err){
                fprintf(stderr,"AppendFile::append() failed %s\n",strerror_tl(err));
                break;
            }
        }
        written += thisTimeWrite;
    }
    m_writtenBytes += written;
}

size_t CzyNetFrame::AppendFile::write(const char *charLine, size_t len)
{   
    ///无锁版本的fwrite,提高写入速度
    return ::fwrite_unlocked(charLine,1,len,m_fp);
}

void AppendFile::flush(){
    ::fflush(m_fp);
}

CzyNetFrame::ReadSmallFile::ReadSmallFile(StringArg fileName)
    :m_fd(::open(fileName.c_str(),O_RDONLY | O_CLOEXEC)),
    m_err(0)
{
    m_buffer[0] = '\0';
    if(m_fd < 0){
        m_err = errno;
    }
}

CzyNetFrame::ReadSmallFile::~ReadSmallFile()
{
}

int CzyNetFrame::ReadSmallFile::readToBuffer(int *size)
{
    int err = m_err;
    ///在标准IO关闭的情况下是可以为0的
    if(m_fd >= 0){
        ///pread用于原子的从文件中带偏移量的读取数据
        ssize_t n = ::pread(m_fd,m_buffer,sizeof(m_buffer) - 1,0);
        if(n >= 0){
            *size = static_cast<int>(n);
        }
        m_buffer[n] = '\0';
    }
    else{
        err = errno;
    }
    return err;
}
