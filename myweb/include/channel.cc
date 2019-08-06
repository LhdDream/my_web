#include "channel.h"
#include "Buffer.h"
#include <functional>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

const int kReadEvent = EPOLLIN | EPOLLPRI ;


void channel::handleEvent() {
   //通过weak_ptr 来进行判断，tcpconnection  有没有连接在上面
    if( events_   &= kReadEvent ) {
        if(readcallback_) // 必须判断
        {
            readcallback_();
        }
    }
}
void channel::handleRead(  Callback cb)
{

    readcallback_ =  cb;
}
void channel::enable_read() {
    events_  = kReadEvent  ; ownloop_->update(this);
}

