///
/**
 * FileUtil是最底层的文件类，封装了Log文件的打开、写入并在类析构的时候关闭文件，
 * 底层使用了标准IO，该append函数直接向文件写。
 *  
 * **/
#pragma once
#include "string"
#include "./nocopy.hpp" 
#define FLASH_LEN (60*1024)
class AppendFile: nocpoy
{
public:
    AppendFile(const std::string& filename);
    ~AppendFile();
    void append(const char* str,size_t len);
    size_t write(const char* logline, size_t len);
    void flush();
    off_t writtenBytes() const { return m_writtenBytes; }
private:
    std::string m_sBaseName;
    size_t m_uFlash_EveryN;
    FILE* m_pFle;
    char m_aBuffer[FLASH_LEN];
    off_t m_writtenBytes;
};