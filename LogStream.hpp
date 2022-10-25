#ifndef MUDUO_BASE_LOGSTREAM_H
#define MUDUO_BASE_LOGSTREAM_H
#include "unistd.h"
#include <string.h>
#include <string>
#include <algorithm>
#include "nocopy.hpp"

/*
整数转string

*/
namespace detail
{
    const int kSmallBuffer = 4000;
    const int kLargeBuffer = 4000 * 100;

    const char digits[] = "9876543210123456789";
    const static char *zero = digits + 9;

    /*
    缓冲区类

    */
    template <int SIZE>
    class FixedBuffer : nocpoy
    {

    public:
        FixedBuffer() : m_pCur(m_aData)
        {
        }
        ~FixedBuffer()
        {
            // delete m_pCur;
        }
        void Append(const char *buf, size_t len)
        {
            if (Avail() > static_cast<int>(len))
            {
                memcpy(m_pCur, buf, len);
                m_pCur += len;
            }
        }

        const char *Data() const
        {
            return m_aData;
        }

        char *Current()
        {
            return m_pCur;
        }
        int Length() const
        {
            return static_cast<int>(m_pCur - m_aData);
        }

        void Add(size_t len)
        {
            m_pCur += len;
        }

        int Avail() const
        {
            auto temp = End() - m_pCur;
            return static_cast<int>(temp);
        }

        void Bzero()
        {
            memset(m_aData, 0, sizeof(m_aData));
        }
        void Rest()
        {
            m_pCur = m_aData;
        }

    private:
        const char *End() const
        {
            const char *temp = m_aData;
            return temp + sizeof(m_aData);
        }
        char m_aData[SIZE];
        char *m_pCur;
    };

    template <typename T>
    size_t converter(char buf[], T value)
    {
        char *p = buf;
        T i = value;
        do
        {
            /* code */
            int lsd = static_cast<int>(i % 10);
            i /= 10;
            *p++ = zero[lsd];
        } while (i != 0);

        if (value < 0)
        {
            *p++ = '-';
        }
        *p = '\0';

        std::reverse(buf, p);
        return p - buf;
    }

}

/*
内容格式化输出

*/
class LogStream : nocpoy
{

    typedef LogStream self;

public:
    typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;

    template <typename T>
    void FormatInteger(T value)
    {
        // std::string str = std::to_string(value);
        size_t len = detail::converter(m_buffer.Current(), value);
        m_buffer.Add(len);
        // detail::converter(m_buffer.Data(),value)
    }
    self &operator<<(bool parm)
    {
        m_buffer.Append(parm ? "1" : "0", 1);
        return *this;
    }

    self &operator<<(int parm)
    {
        FormatInteger(parm);
        return *this;
    }

    self &operator<<(unsigned int parm)
    {
        FormatInteger(parm);
        return *this;
    }
    self &operator<<(short parm)
    {
        FormatInteger(parm);
        return *this;
    }
    self &operator<<(unsigned short parm)
    {
        FormatInteger(parm);
        return *this;
    }

    self &operator<<(long parm)
    {
        FormatInteger(parm);
        return *this;
    }

    self &operator<<(unsigned long parm)
    {
        FormatInteger(parm);
        return *this;
    }
    self &operator<<(long long parm)
    {
        FormatInteger(parm);
        return *this;
    }
    self &operator<<(unsigned long long parm)
    {
        FormatInteger(parm);
        return *this;
    }
    self &operator<<(const void *parm)
    {
        // FormatInteger(parm);
        return *this;
    }

    self &operator<<(float v)
    {
        *this << static_cast<double>(v);
        return *this;
    }

    self &operator<<(double value)
    {
        if (m_buffer.Avail() >= kMaxNumericSize)
        {

            int len = snprintf(m_buffer.Current(), kMaxNumericSize, "%.12g", value);
            m_buffer.Add(len);
        }
        return *this;
    }
    self &operator<<(char value)
    {
        if (value)
        {
            m_buffer.Append(&value, 1);
        }
        else
        {
            m_buffer.Append("(null)", 6);
        }
        return *this;
    }
    self &operator<<(const char *value)
    {
        if (value)
        {
            m_buffer.Append(value, strlen(value));
        }
        else
        {
            m_buffer.Append("(null)", 6);
        }
        return *this;
    }
    self &operator<<(const unsigned char *str)
    {
        return operator<<(reinterpret_cast<const char *>(str));
    }
    self &operator<<(const std::string &v)
    {
        m_buffer.Append(v.c_str(), v.size());
        return *this;
    }
    //  self& operator<<(const Buffer& value)
    //  {

    //  }

    void Append(const char *data, int len)
    {
        m_buffer.Append(data, len);
    }
    const Buffer &buffer() const
    {
        return m_buffer;
    }

    void RestBuffer()
    {
        m_buffer.Rest();
    }

    static const int kMaxNumericSize = 48;

private:
    Buffer m_buffer;
};

#endif
