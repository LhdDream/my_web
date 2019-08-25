#include "Buffer.h"
#include <unistd.h>
#include <string.h>
#include <sys/uio.h>
#include <string>
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
    ssize_t  n = 0;
    while(1) {
        n = readv(fd, vec, 2);
        n = (int) n;
        if (n < 0) {
            if (errno == EAGAIN)
                continue;
            else
                return -1;
        }
        else
            break;
    }
 //.   printf("wirte ......  %d %d %zu\n",fd,n,writable);
    n = (ssize_t) n;
    if ( n <= writable) {
        writeindex += n;
    } else {
        writeindex = data_.size();
        append_(extrabuf, n - writable);
    }
    return n;
}



