#pragma once
#include <sys/types.h>
#include "src/base/NonCopyAble.h"
#include "src/base/StringPiece.h"
#include <filesystem>
namespace{
    /// @brief 64kbFile
    constexpr int KSmallBufferSize = 64 * 1024;
}

namespace CzyNetFrame{

class ReadSmallFile :public NonCopyAble
{
public:
    ReadSmallFile(StringArg fileName);
    ~ReadSmallFile();
    
    
    template<typename String>
    int readToString(int maxSize,
                     String *content,
                     int64_t *fileSize,
                     int64_t *modifyTime,
                     int64_t *createTime);

    int readToBuffer(int *size);

    const char *buffer() const{return m_buffer;}


private:
    std::filesystem::path m_path;
    int m_fd;
    int m_err;
    char m_buffer[KSmallBufferSize];
};

template<typename String>
int readFile(StringArg fileName,
            int maxSize,
            String *content,
            int64_t *fileSize = NULL,
            int64_t *modifyTime = NULL,
            int64_t *createTime = NULL)
{
    ReadSmallFile file(fileName);
    return file.readToString(maxSize,content,fileSize,modifyTime,createTime);
}

class AppendFile :public NonCopyAble
{
public:
    explicit AppendFile(StringArg fileName);

    ~AppendFile();

    void append(const char *logLine ,size_t len);

    void flush();


    off_t writtenBytes() const{return m_writtenBytes;}
private:   
    
    size_t write(const char *charLine,size_t len);


private:
    FILE* m_fp;
    char m_buffer[KSmallBufferSize];
    off_t m_writtenBytes{0};
};

template <typename String>
inline int ReadSmallFile::readToString(int maxSize, String *content, int64_t *fileSize, int64_t *modifyTime, int64_t *createTime)
{
   static_assert(sizeof(off_t) == 8,"FILE_OFFSET_BITS = 64");
   assert(content != NULL);
   int err = m_err; /*
   if(m_fd >= 0){
        content->clear();
        if(fileSize){
            ///±£´æÎÄ¼þ×´Ì¬
            std::filesystem::file_status status(m_path);
            if(status.type() == std::filesystem::is_regular_file){
                *fileSize = static_cast<int64_t>(std::filesystem file_size(m_path));
                content.reserve(*fileSize);
            }
            else if(status.type() == std::filesystem::is_directory){
                err = EISDIR;
            }
            if(modifyTime){
                *modifyTime = std::filesystem::last_write_time(m_path).time_since_epoch().count();
            }
        }
   }
   */
   return 0;
}

}
