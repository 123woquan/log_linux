
#pragma once
#include "FileUtile.h"
#include "nocopy.hpp"
#include "shared_mutex"
#include "memory"
#include "mutex"
#include <unistd.h>
class LogFIle : nocpoy
{
private:
    /* data */
    std::unique_ptr<AppendFile>  m_pFile;
    const std::string m_sfilename;
    time_t m_tLastFlush_;
    int m_nFlushInterval;
    off_t m_rollSize;
    std::unique_ptr<std::mutex> m_pMtx;
    int m_nindex; //日志文件名编号
    time_t m_lastRoll_;
    const static int RollSeconds_ = 60*60*24;
public:
//每被append flushEveryN次，flush一下，会往文件写，只不过，文件也是带缓冲区的
    LogFIle(const std::string filename,int flush,off_t rollsize);
    ~LogFIle();

    void Init();
    void Flush();
    void Append(const char* logline,int len);
    void append_unlocked(const char* logline, int len);
    std::string GetFileName(const std::string& filebasename,time_t* now);
    bool rollFile();
    std::string hostname()
    {
        // HOST_NAME_MAX 64
        // _POSIX_HOST_NAME_MAX 255
        char buf[256];
        if (::gethostname(buf, sizeof buf) == 0)
        {
            buf[sizeof(buf)-1] = '\0';
            return buf;
        }
        else
        {
            return "unknownhost";
        }
    }

};

