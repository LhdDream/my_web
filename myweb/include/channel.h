//
// Created by kiosk on 19-7-30.
//

#ifndef MYWEB_CHANNEL_H
#define MYWEB_CHANNEL_H
//IO事件分发器，对于IO操作负责不同的回调
#include <functional>
#include "Eventloop.h"
class Eventloop;
class channel
{
    public:
    channel(int fd):fd_(fd){
    };
    ~channel(){};
        void handleEvent();
        int fd() const {return fd_;}
        void  set_events(int events) {  events_ = events; }
        void handleRead();
        void  set_fd(int fd ) {fd_ =fd ;}
    private:
        int fd_;
        int events_;

};
#endif //MYWEB_CHANNEL_H
