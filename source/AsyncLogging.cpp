#pragma once
#include "AsyncLogging.h"
#include <assert.h>
#include "chrono"
#include "iostream"
using namespace std::chrono_literals;
// AsyncLogging::AsyncLogging(const std::string& filename,off_t rollSize)
// {

// }

AsyncLogging::~AsyncLogging()
{
    if (m_bRunning)
    {
        Stop();
    }
}

void AsyncLogging::Append(const char *logline, int len)
{
    std::lock_guard<std::mutex> lg(m_mtx);
   // std::cout<<"AsyncLogging::Append start"<<std::endl;
    if (m_currentBuffer->Avail() > len)
    {
          //std::cout<<"AsyncLogging::Append valid"<<std::endl;
        m_currentBuffer->Append(logline, len);
    }
    else
    {
       // std::cout<<"AsyncLogging::Append is full"<<std::endl;
        m_buffers.push_back(std::move(m_currentBuffer));
        if (m_nextBuffer)
        {
            m_currentBuffer = std::move(m_nextBuffer);
        }
        else
        {
            m_currentBuffer.reset(new Buffer());
        }
        m_currentBuffer->Append(logline, len);
        m_cv.notify_all();
       //  std::cout<<"AsyncLogging::Append end"<<std::endl;
    }
}
void AsyncLogging::threadFunc()
{
    LogFIle output(m_sBasemae, m_nFlushInterval, m_rollSize);
    //Bufferptr newbuffer1 = std::make_unique<Buffer>(new Buffer());
   // Bufferptr newbuffer2 = std::make_unique<Buffer>(new Buffer());
    Bufferptr newbuffer1(new Buffer());
    Bufferptr newbuffer2(new Buffer());
   // std::unique_ptr<Buffer> buf = std::make_unique<Buffer>(new Buffer());
    newbuffer1->Bzero();
    newbuffer2->Bzero();
    BufferVector bufferVectortoWrite;
    bufferVectortoWrite.reserve(16);
    while (m_bRunning)
    {
        // assert(newbuffer1 && newbuffer1->Length()==0);
        // assert(newbuffer2 && newbuffer2->Length()==0);
        // assert(bufferVectortoWrite.empty());
       // std::cout<<"AsyncLogging::threadFunc start"<<std::endl;
        std::unique_lock<std::mutex> ul(m_mtx);
        if (m_buffers.empty())
        {
            auto now = std::chrono::system_clock::now();

             m_cv.wait_until(ul, now + m_nFlushInterval * 100ms,[&]()
                             { return !m_buffers.empty(); });
            // m_cv.wait(m_mtx,!m_buffers.empty());
        }
       // std::cout<<"AsyncLogging::threadFunc cond"<<std::endl;
        m_buffers.push_back(std::move(m_currentBuffer));
        m_currentBuffer = std::move(newbuffer1);
        bufferVectortoWrite.swap(m_buffers);
        if (!m_nextBuffer)
        {
            m_nextBuffer = std::move(newbuffer2);
        }
        ul.unlock();
        for (const auto &item : bufferVectortoWrite)
        {
            output.Append(item->Data(), item->Length());
        }
        if (bufferVectortoWrite.size() > 2)
        {
            bufferVectortoWrite.resize(2);
        }

        if (!newbuffer1)
        {
            assert(!bufferVectortoWrite.empty());

            newbuffer1 = std::move(bufferVectortoWrite.back());
            bufferVectortoWrite.pop_back();
            newbuffer1->Rest();
        }
        if (!newbuffer2)
        {
            assert(!bufferVectortoWrite.empty());

            newbuffer2 = std::move(bufferVectortoWrite.back());
            bufferVectortoWrite.pop_back();
            newbuffer2->Rest();
        }
        bufferVectortoWrite.clear();
        output.Flush();
        //std::cout<<"AsyncLogging::threadFunc end"<<std::endl;
    }
    output.Flush();
}
