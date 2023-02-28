#pragma once
#include <sys/types.h>
#include "src/base/NonCopyAble.h"
#include "src/base/StringPiece.h"
#include <unistd.h>
namespace
{
    /// @brief 64kbFile
    constexpr int KSmallBufferSize = 64 * 1024;
}

namespace CzyNetFrame
{

    class ReadSmallFile : public NonCopyAble
    {
    public:
        ReadSmallFile(StringArg fileName);
        ~ReadSmallFile();

        template <typename String>
        int readToString(int maxSize,
                         String *content,
                         int64_t *fileSize,
                         int64_t *modifyTime,
                         int64_t *createTime);

        int readToBuffer(int *size);

        const char *buffer() const { return m_buffer; }

    private:
        int m_fd;
        int m_err;
        char m_buffer[KSmallBufferSize];
    };

    template <typename String>
    int readFile(StringArg fileName,
                 int maxSize,
                 String *content,
                 int64_t *fileSize = NULL,
                 int64_t *modifyTime = NULL,
                 int64_t *createTime = NULL)
    {
        ReadSmallFile file(fileName);
        return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
    }

    class AppendFile : public NonCopyAble
    {
    public:
        explicit AppendFile(StringArg fileName);

        ~AppendFile();

        void append(const char *logLine, const size_t len);

        void flush();

        off_t writtenBytes() const { return m_writtenBytes; }

    private:
        size_t write(const char *charLine, size_t len);

    private:
        FILE *m_fp;
        char m_buffer[KSmallBufferSize]{0};
        off_t m_writtenBytes{0};
    };

    template <typename String>
    inline int ReadSmallFile::readToString(int maxSize, String *content, int64_t *fileSize, int64_t *modifyTime, int64_t *createTime)
    {
        static_assert(sizeof(off_t) == 8, "FILE_OFFSET_BITS = 64");
        assert(content != NULL);
        int err = m_err;

        if (m_fd >= 0)
        {
            content->clear();
            if (fileSize)
            {
                struct stat fileStatBuf;
                if (fstat(m_fd, &fileStatBuf) == 0)
                {
                    /// 检查linux系统调用API的返回结果
                    if (S_ISREG(fileStatBuf.st_mode))
                    {
                        *fileSize = fileStatBuf.st_size;
                        content.reserve(*fileSize);
                    }
                    else if (S_ISDIR(fileStatBuf.st_mode))
                    {
                        err = EISDIR;
                    }
                    if (modifyTime)
                    {
                        *modifyTime = fileStatBuf.st_mtime;
                    }
                    if (createTime)
                    {
                        *createTime = fileStatBuf.st_ctime;
                    }
                }
            }
            else
            {
                err = errno;
            }

            while (content.size() < static_cast<size_t>(maxSize))
            {
                size_t toRead = std::min(static_cast<size_t>(maxSize) - content.size(), sizeof(m_buffer));
                size_t n = ::read(m_fd, m_buffer, toRead);
                if (n > 0)
                {
                    content->append(m_buffer, n);
                }
                else
                {
                    if (n < 0)
                    {
                        err = errno;
                    }
                    break;
                }
            }
            return err;
        }
        return 0;
    }

}
