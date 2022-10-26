#pragma once
#include "Logging.h"
#include "AsyncLogging.h"
#include "thread"
#include "mutex"
time_t t_lastSecond;
char t_time[64];
static AsyncLogging* g_AsyncLogger;
static std::once_flag g_once_flag;

class T
{
 public:
  T(const char* str, unsigned len)
    :str_(str),
     len_(len)
  {

  }

  const char* str_;
  const unsigned len_;
};
const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
{
  "TRACE ",
  "DEBUG ",
  "INFO  ",
  "WARN  ",
  "ERROR ",
  "FATAL ",
};

inline LogStream& operator<<(LogStream& s, T v)
{
  s.Append(v.str_, v.len_);
  return s;
}
void init()
{
    std::string filepath ="./Serve.log";
    off_t len = 10*1024*1000;
    //off_t len = 10*1024;
   // Logger::Impl::g_ppid = ::getppid();
    g_AsyncLogger = new AsyncLogging(filepath,len,3);
   // g_AsyncLogger = new AsyncLogging(filepath,len,3);
   
    g_AsyncLogger->Start();
}

void output(const char* msg, int len)
{
    std::call_once(g_once_flag,init);
    g_AsyncLogger->Append(msg,len);

}


Logger::~Logger()
{
    m_impl.Finish();
    const LogStream::Buffer& buf(m_impl.m_stream.buffer());
    output(buf.Data(),buf.Length());
}
int Logger::Impl::g_ppid=::getppid();;
Logger::Impl::Impl(Loglevel level,int old_errno,const SourceFile &file,int line):m_level(level),
                                                                                m_baseName(file),
                                                                                m_nLine(line)

{
    FormatTime();   
    tid();
    m_stream<<T(tidString(),tidStringLength());
    m_stream<<LogLevelName[level];
   // m_stream<<tidString(),tidStringLength();
}
 void Logger::Impl::Finish()
 {

    m_stream<<"-"<<m_baseName.GetFileName()<<":"<<m_nLine<<"\n";


 }
 Logger::Impl::~Impl()
 {

 }


//  void Logger::Impl::FormatTime()
//  {

//    struct timeval tv;
//    time_t time;
//    char str_t[26]={0};
//    gettimeofday(&tv, NULL);
//    time = tv.tv_sec;
//    //struct tm* p_time = localtime(&time);
//    strftime(str_t,26,"%Y-%m-%d %H:%M:%S ",p_time);
//    m_stream<<str_t;
//  }
 void Logger::Impl::FormatTime()
 {
    struct timeval tv;
    //time_t time;
    char str_t[26]={0};
    gettimeofday(&tv, NULL);
   // time_t seconds = tv.tv_sec;
   // struct tm tm_time;
    long long ms =static_cast<long long>(tv.tv_usec / 1000);


    long long temp = tv.tv_sec*1000+tv.tv_usec / 1000;
    m_stream<<temp;
 }
