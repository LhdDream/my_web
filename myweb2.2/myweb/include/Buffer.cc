#include "Buffer.h"
ssize_t Buffer::readfd(int fd)
{
    //选择栈空间，尽量一次进行读取完毕
    char extrabuf[65520];
    const size_t writable = writeableByte();
    int  n = 0;
    while(true) {
        bzero(extrabuf,sizeof extrabuf);
        n = recv(fd,extrabuf,sizeof(extrabuf),0);
        if (n < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            else
                return -1;
        }
        else
            break;
    }
    if ( n <= writable) {
      append_(extrabuf,n);
    } else {
        writeindex = data_.size();
        append_(extrabuf, n - writable);
    }
    return n;
}



