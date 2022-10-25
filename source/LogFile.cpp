#include "LogFIle.h"
LogFIle::LogFIle(const std::string filename,int flush,off_t rollsize):m_sfilename(filename),
                                                        m_tLastFlush_(0),
                                                        m_nFlushInterval(flush),
                                                        m_rollSize(rollsize),
                                                        m_lastRoll_(0),
                                                        m_nindex(0)
                                                        
{
    Init();

}

LogFIle::~LogFIle()
{
}
void LogFIle::Init()
{
     m_pMtx = std::make_unique<std::mutex>();
     m_pFile = std::make_unique<AppendFile>(m_sfilename); //，使用 std::make_unique 是一个好习惯,解决了一些异常安全问题
}

void LogFIle::Flush()
{
    std::lock_guard<std::mutex> _lock(*m_pMtx);
    m_pFile->flush();
}

void LogFIle::Append(const char* logline,int len)
{
     std::lock_guard<std::mutex> _lock(*m_pMtx);

     append_unlocked(logline,len);
}

void LogFIle::append_unlocked(const char* logline, int len)
{
    m_pFile->append(logline,len);
    if(m_pFile->writtenBytes()>m_rollSize)  //定量写入到文件中
    {
        rollFile();
        m_pFile->flush();
    }
    else
    {
        time_t _now = time(NULL);
        if((_now/RollSeconds_) != (m_lastRoll_/RollSeconds_))  //tm/24*60*60  判断两个时间是不是同一天
        {
            rollFile();
        }
        else if (_now - m_tLastFlush_ > m_nFlushInterval) //定时写入到文件中
        {
            m_tLastFlush_ = _now;
            m_pFile->flush();
        }
    }
    

}


 std::string LogFIle::GetFileName(const std::string &filebasename,time_t* now)
 {
    std::string filename;
    filename.reserve(filebasename.size()+64);
    filename = filebasename;
    char timebuf[32];
    struct tm tm;
    *now = time(NULL);
    gmtime_r(now,&tm);
    strftime(timebuf,sizeof(timebuf),".%Y%m%d-%H:%M:%S",&tm);

    filename+=timebuf;
    if((*now/RollSeconds_) != (m_lastRoll_/RollSeconds_))  //tm/24*60*60  判断两个时间是不是同一天
    {
        m_nindex =0;
        m_lastRoll_=*now;
    }
    filename+=("-"+std::to_string(m_nindex)+".");
    filename+=hostname();
    char pidbuf[32];
    snprintf(pidbuf, sizeof pidbuf, ".%d", ::getpid());
    filename+=pidbuf;
    filename+=".log";
    m_nindex++;
    return filename;
 }
  bool LogFIle::rollFile()
  {
    time_t now = 0;

    std::string filename = GetFileName(m_sfilename,&now);
    m_pFile.reset(new AppendFile(filename));
    return true;
  }
  