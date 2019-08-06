#include "Buffer.h"
#include <unistd.h>
#include <string.h>
#include <sys/uio.h>
ssize_t Buffer::readfd(int fd)
{
    //选择栈空间，尽量一次进行读取完毕
    char extrabuf[65520];
    struct iovec vec[2];
    const size_t writable = writeableByte();
    vec[0].iov_base = &*data_.begin() + writeindex;
    vec[0].iov_len = writable;
    vec[1].iov_base = extrabuf;//保存的地址
    vec[1].iov_len = sizeof(extrabuf);
    const ssize_t n = readv(fd, vec, 2);
    if ( n <= writable) {
        writeindex += n;
    } else {
        writeindex = data_.size();
        append(extrabuf, n - writable);
    }
    return n;
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


