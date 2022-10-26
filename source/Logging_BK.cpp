//  #pragma once
// #include "Logging_bk.h"

// #include "Logging.h"
// #include "AsyncLogging.h"
// #include "thread"
// #include "mutex"
// time_t t_lastSecond;
// char t_time[64];
// static AsyncLogging* g_AsyncLogger;
// static std::once_flag g_once_flag;
// void init()
// {
//     std::string filepath ="./Serve.log";
//     off_t len = 10240;
//     g_AsyncLogger = new AsyncLogging(filepath,len,1);
//    // g_AsyncLogger = new AsyncLogging(filepath,len,3);
//     g_AsyncLogger->Start();
// }

// void output(const char* msg, int len)
// {
//     std::call_once(g_once_flag,init);
//     g_AsyncLogger->Append(msg,len);

// }


// Logger::~Logger()
// {
//     m_impl.Finish();
//     const LogStream::Buffer& buf(m_impl.m_stream.buffer());
//     output(buf.Data(),buf.Length());
// }

// Logger::Impl::Impl(Loglevel level,int old_errno,const SourceFile &file,int line):m_level(level),m_baseName(file),m_nLine(line)
// {
//     FormatTime();
// }
//  void Logger::Impl::Finish()
//  {

//     m_stream<<"-"<<m_baseName.GetFileName()<<":"<<m_nLine<<"\n";

//  }
//  Logger::Impl::~Impl()
//  {
   
//  }    
 
 
//  void Logger::Impl::FormatTime()
//  {

//    struct timeval tv;
//    time_t time;
//    char str_t[26]={0};
//    gettimeofday(&tv, NULL);
//    time = tv.tv_sec;
//    struct tm* p_time = localtime(&time);
//    strftime(str_t,26,"%Y-%m-%d %H:%M:%S ",p_time);
//    m_stream<<str_t;
//  }