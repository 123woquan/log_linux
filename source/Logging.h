#pragma once
#include <stdio.h>
#include <unistd.h>
#include "string.h"
#include "LogStream.hpp"
#include <sys/time.h>
#include <sys/syscall.h>
class SourceFile;
class Impl;

class Logger
{
public:
    enum LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };
    //编译期间获取文件名
    class SourceFile
    {
    private:
        /* data */
        const char *m_pData;  //m_pData 是由外部传入的，非本类管理就不用本类自己释放
        int m_nSize;

    public:
        template <int N>
        SourceFile(const char (&data)[N]) : m_pData(data), m_nSize(N - 1)
        {
            const char *filename = strrchr(m_pData, '/');
            if (filename) // have path,such as a/b/name.txt
            {
                m_pData = filename + 1;
                m_nSize = static_cast<int>(m_pData - filename);
            }
        }

        explicit SourceFile(const char *filename) : m_pData(filename)
        {
            const char *name = strrchr(m_pData, '/');
            if (name) // have path,such as a/b/name.txt
            {
                m_pData = name + 1;
                m_nSize = static_cast<int>(m_pData - name);
            }
        }

        ~SourceFile()
        {
          //  delete m_pData;
        }
        const char *GetFileName()
        {
            return m_pData;
        }
    };

    class Impl
    {
    public:
        typedef Logger::LogLevel Loglevel;
        Impl(Loglevel level, int old_errno, const SourceFile &file, int line);
        ~Impl();
        void FormatTime();
        void Finish();

        Loglevel m_level;
        int m_nLine;
        SourceFile m_baseName;
        LogStream m_stream;
        int t_cachedTid=0;  //Thread id
        char t_tidString[64];
        int t_tidStringLength;

        pid_t gettid()  //Get Thread id
        {
            return static_cast<pid_t>(::syscall(SYS_gettid)); //::syscall(SYS_gettid) get real thread id
        }
        int tid()
        {
            if (__builtin_expect(t_cachedTid == 0, 0))
            {
                if (t_cachedTid == 0)
                {
                    t_cachedTid = ::gettid();

                    t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "(%d:%5d) ",g_ppid,t_cachedTid);
                    //snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid); //format threadid
                }
            }
            return t_cachedTid;
        }

        const char* tidString() // for logging
        {
            return t_tidString;
        }
        int tidStringLength() // for logging
        {
            return t_tidStringLength;
        }
        static int g_ppid;
    };


private:
    /* data */
    Impl m_impl;


public:
    Logger(SourceFile fileName, int line) : m_impl(INFO, 0, fileName, line)
    {
    }
    Logger(SourceFile fileName, int line, LogLevel level) : m_impl(level, 0, fileName, line)
    {
    }
    Logger(SourceFile fileName, int line, LogLevel level, const char *func) : m_impl(level, 0, fileName, line)
    {
    }
    // Logger(SourceFile& fileName, int line);

    ~Logger();

    LogStream &stream()
    {

        return m_impl.m_stream;
    }
};

#define LOG_TRACE Logger(__FILE__, __LINE__, Logger::TRACE).stream()
#define LOG_INFO Logger(__FILE__, __LINE__, Logger::INFO).stream()
#define LOG_WARN Logger(__FILE__, __LINE__, Logger::WARN).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__, Logger::ERROR).stream()
