#pragma once
#include "nocopy.hpp"
#include "LogFIle.h"
#include "LogStream.hpp"
#include <vector>
#include <atomic>
#include "thread"
#include "condition_variable"
#include "unistd.h"
#include "memory.h"
#include "chrono"
class AsyncLogging:nocpoy
{
private:
    /* data */
public:
    AsyncLogging(const std::string& filename, off_t rollSize,int flushev): m_sBasemae(filename),
                                                                           m_rollSize(rollSize),
                                                                           m_currentBuffer(new Buffer),
                                                                           m_nextBuffer(new Buffer()),
                                                                           m_nFlushInterval(flushev)
    {
        Init();
    }
    ~AsyncLogging();
    void Init()
    {
       // m_currentBuffer = std::make_unique<Buffer>(new Buffer());
        m_currentBuffer->Bzero();
      //  m_nextBuffer = std::make_unique<Buffer>(new Buffer());
        m_nextBuffer->Bzero();
    }

    void Start()
    {
        m_bRunning = true;
        std::thread trd(&AsyncLogging::threadFunc,this);
        
        trd.detach();
    }
    void Stop()
    {
        m_bRunning =false;
        
        


    }
    void Append(const char* logline,int len);
    /*
    首先准备好两块空闲的buffer，已备在临界区内交换，等待条件标量出发的条件又两个，
    超时或者是前端写满了一个或者多个Buffer,当条件满足时，先将当前缓冲移入buffer，
    并且立刻将空闲的newBuffer1作为当前缓冲，接下来将buffers和buffersToWrite交
    换，随后将buffersToWrite写入文件，重新设计设置Buffer
    */
    void threadFunc();

private:
    std::unique_ptr<LogFIle> m_pLogFile;
    typedef detail::FixedBuffer<detail::kLargeBuffer> Buffer;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
    typedef BufferVector::value_type Bufferptr;
    
    const int m_nFlushInterval;
    const std::string m_sBasemae;

    std::atomic<bool> m_bRunning;
    std::thread m_thrd;
    std::mutex m_mtx;
    std::condition_variable m_cv;
    const off_t m_rollSize;
    Bufferptr m_currentBuffer;
    Bufferptr m_nextBuffer;
    
    BufferVector m_buffers;


};


