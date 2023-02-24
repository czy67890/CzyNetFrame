#include "src/base/FileUnit.h"
#include "src/base/Log.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "FileUnit.h"

using namespace CzyNetFrame;
AppendFile::AppendFile(StringArg fileName)
    :m_fp(::fopen(fileName.c_str(),"ae"))
{
    assert(m_fp);
    ::setbuffer(m_fp,m_buffer,sizeof(m_buffer));
}

AppendFile::~AppendFile(){
    ::fclose(m_fp);
}

void AppendFile::append(const char *charLine,size_t size){
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
            written += thisTimeWrite;
        }
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
    :m_path{fileName.c_str()}
{
    
}

CzyNetFrame::ReadSmallFile::~ReadSmallFile()
{
}

int CzyNetFrame::ReadSmallFile::readToBuffer(int *size)
{
    return 0;
}
