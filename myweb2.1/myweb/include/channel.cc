#include "channel.h"

const int kReadEvent = EPOLLIN ;
const int WriteEvent = EPOLLOUT;
void channel::handleEvent() {
   //通过weak_ptr 来进行判断，tcpconnection  有没有连接在上面
  // printf("channel  fd %d \n",fd_);
  //  printf("begin  channel\n");
    if( events_  & kReadEvent ) {
        if(readcallback_) // 必须判断
        {
          readcallback_();
        }
    }
    if( events_ & WriteEvent)
    {
        if(writecallback_)
        {
            writecallback_();
        }
    }
}
void channel::handleRead( const Callback& cb)
{
    readcallback_ =  cb;
}
void channel::handleWrite(const Callback &cb)
{
    writecallback_ = cb;
}

void channel::enable_write()
{
    events_ |= WriteEvent;
    ownloop_->update(this);
}
void channel::enable_ET() {
    events_ |= EPOLLET;
    events_ |= EPOLLIN;
    ownloop_->update(this);
}

