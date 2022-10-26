#pragma once
#include <stdio.h>
#include "string.h"
#include "LogStream.hpp"
#include <sys/time.h>
#include "AsyncLogging.h"
#include "thread"
#include "mutex"
namespace Logging
{

    static AsyncLogging* g_AsyncLogger;
    static std::once_flag g_once_flag;
    void init()
    {
        std::string filepath ="./Serve.log";
        off_t len = 10240;
        g_AsyncLogger = new AsyncLogging(filepath,len,1);
    // g_AsyncLogger = new AsyncLogging(filepath,len,3);
        g_AsyncLogger->Start();
    }

    void output(const char* msg, int len)
    {
        std::call_once(g_once_flag,init);
        g_AsyncLogger->Append(msg,len);

    }


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

    typedef LogLevel Loglevel;
    template <int N>
    void SourceFile(const char* data,char *filename)
   { 
        //int m_nSize =N-1;
        // filename = strrchr(data, '/');
        // if (filename) // have path,such as a/b/name.txt
        // {
        //     filename = filename + 1;
        //    // m_nSize = static_cast<int>(m_pData - filename);
        // }
    
    }
    void FormatTime(LogStream& m_stream)
    {
        struct timeval tv;
        time_t time;
        char str_t[26]={0};
        gettimeofday(&tv, NULL);
        time = tv.tv_sec;
        struct tm* p_time = localtime(&time);
        strftime(str_t,26,"%Y-%m-%d %H:%M:%S ",p_time);
        m_stream<<str_t;
    }
   
    void Impl(Loglevel level, int old_errno, const char* file, int line,LogStream& strm)
    {
        //LogStream m_stream;
        
        FormatTime(strm);
        char* tempname =new char();
       // SourceFile(file,tempname);

        strm<<"-"<<file<<":"<<line<<"\n";
    }
   class CLog
   {
    public:
    template <int N>
        CLog(const char (&data)[N], int line) 
        {
           
        }
        template <int N>
        CLog(const char (&data)[N], int line, LogLevel level) : m_le(level),m_fileName(data),m_line(line)
        { 
           
        }
        template <int N>
        CLog(const char (&data)[N], int line, LogLevel level, const char *func)
        {
        }
        ~CLog()
        {
            Impl(m_le,0,m_fileName,m_line,m_stream);
             const LogStream::Buffer& buf(m_stream.buffer());
             output(buf.Data(),buf.Length());
        }
        template<class T>
        CLog& operator<<(T value)
        {
            m_stream<<value;
            return *this;
        }
    private:
     LogStream m_stream;
     LogLevel m_le;
     const char* m_fileName;
     int m_line;
   };
 #define LOG_INFO CLog(__FILE__, __LINE__, Logging::INFO)
}
// #define LOG_TRACE Logging::CLog(__FILE__, __LINE__, Logging::LogLevel::TRACE)

// #define LOG_WARN Logging::CLog(__FILE__, __LINE__, Logging::TRACE)
// #define LOG_ERROR Logging::CLog(__FILE__, __LINE__, Logging::TRACE)