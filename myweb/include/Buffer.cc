#include "Buffer.h"
#include <unistd.h>
#include <string.h>
size_t Buffer::readfd(char * vptr,size_t n) // 读取缓冲区中的内容
{
    n  = (n < readaleByte()) ? n :readaleByte() ;
    size_t i = 0;
    while(n != 0)
    {
        vptr[i] = data_[readindex+i];
        n--;
        i++;
        readindex++;
    }
    return i;
}
size_t Buffer::writefd( char *buf, int n)
{
    resize();
    size_t i = 0;
    while(n != 0)
    {
        data_[writeindex+i] = buf[i];
        n--;
        i++;
        writeindex++;
    }
    return i;
}


