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
   //ͨ��weak_ptr �������жϣ�tcpconnection  ��û������������
    if( events_   &= kReadEvent ) {
        if(readcallback_) // �����ж�
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

