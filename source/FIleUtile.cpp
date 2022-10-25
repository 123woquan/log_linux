#include "FileUtile.h"
AppendFile::AppendFile(const std::string& filename):m_sBaseName(filename),
                                                    m_pFle(fopen(filename.c_str(), "ae")),
                                                    m_writtenBytes(0)
{
    setbuffer(m_pFle,m_aBuffer,sizeof m_aBuffer); // 目的是先写入到缓冲区，然后每次刷新时才写入到文件
}

AppendFile::~AppendFile()
{
    fclose(m_pFle);
}

void AppendFile::append(const char* str,size_t len)
{
   
    size_t writelen =0;
    while(writelen!=len)
    {
       size_t remain = len-writelen;
       size_t n = this->write(str+writelen,len);
        if(n != remain)
        {
           int errorcode = ferror(m_pFle); //
           if(errorcode)
           {
                fprintf(stderr,"append file fail!\n"); // stderr标准错误输出设备 两者默认向屏幕输出
                break;
           }
           
        }
        writelen+=n;
       
    }
     m_writtenBytes+=writelen;
}

size_t AppendFile::write(const char* logline, size_t len)
{
  // #undef fwrite_unlocked
  return ::fwrite_unlocked(logline, 1, len, m_pFle); //  线程不安全，但是性能高
}

void AppendFile::flush()
{
    ::fflush(m_pFle);
    //writelen =0;
}