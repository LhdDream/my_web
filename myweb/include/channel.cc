#include "channel.h"
#include "Buffer.h"
#include <functional>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <sys/sendfile.h>
#include "httpresponse.h"
#include "httprequest.h"
#include "HttpContext.h"
const int kReadEvent = EPOLLIN | EPOLLPRI;


void channel::handleEvent() {

    if( events_ |= kReadEvent)
        channel::handleRead();
}

void channel::handleRead() {
    char buffer[256];
    bzero(buffer,sizeof(char) * 256);
    int ret  = recv(fd_,buffer,sizeof(char) *256,0);
    if(ret < 0)
    {
        perror("read recv error");
    }
    Buffer buf;
    buf.append(buffer,sizeof(char) *256);
    HttpContext context;
    context.parseRequest(&buf);
    HttpResponse resp(context.path());
    resp.getfile(fd_);
}