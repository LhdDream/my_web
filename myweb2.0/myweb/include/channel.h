//
// Created by kiosk on 19-7-30.
//

#ifndef MYWEB_CHANNEL_H
#define MYWEB_CHANNEL_H
//IO事件分发器，对于IO操作负责不同的回调
#include <functional>
#include "Eventloop.h"
#include <iostream>
#include <sys/epoll.h>
class Eventloop;
class channel
{
    public:
    typedef std::function< void() > Callback;
    channel(Eventloop * loop,int fd) : fd_(fd),events_(0),ownloop_(loop){
    };
    ~channel() {};
    void handleEvent();
    int fd() const {return fd_;}
    void enable_write();
    void enable_ET();
    void  set_events(int events) { events_ = events;}
    bool hasnoevent()
    {
        return events_ == 0;
    }
    int get_events(){return events_;};
    void handleRead(const  Callback &cb);
    void handleWrite(const Callback &cb);
    private:
        int fd_;
        int events_;
        std::unique_ptr<Eventloop> ownloop_;
        Callback readcallback_;
        Callback writecallback_;
};
#endif //MYWEB_CHANNEL_H
