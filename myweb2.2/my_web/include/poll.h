//
// Created by kiosk on 19-7-30.
//

#ifndef MYWEB_POLL_H
#define MYWEB_POLL_H//IO 多路复用

#include <sys/epoll.h>
#include <fcntl.h>
#include "Epoll_event.h"
#include "channel.h"
#include "Socket.h"
#include <memory>

//epoll 反应堆模型
//epoll 中的事件可读将IO事件分发出去进行事件处理

class channel;

class Socket;

class poll {
public:
    explicit poll() : epollfd_(::epoll_create1(EPOLL_CLOEXEC)) {
    };

    int add_channel(Epoll_event &&ev) const {
        return epoll_ctl(epollfd_, EPOLL_CTL_ADD, ev.event_fd(), ev.pointer());
    }

    int remove_channel(Epoll_event &&ev) const {
        return epoll_ctl(epollfd_, EPOLL_CTL_DEL, ev.event_fd(), ev.pointer());
    }

    int update_channel(Epoll_event &&ev) const {
        return epoll_ctl(epollfd_, EPOLL_CTL_MOD, ev.event_fd(), ev.pointer());
    }

    //尽可能的使用右值移动构造
    //设置一个epoll_wait
    void Wait(EpollEventResult &result, size_t *user_number) {
        //设置epoll_Wait超时
        *user_number = epoll_wait(epollfd_, result.get(), result.fillsize_(), -1);
    }

private:
    int epollfd_;
};

#endif //MYWEB_POLL_H
