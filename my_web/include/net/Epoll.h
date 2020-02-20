//
// Created by kiosk on 19-7-30.
//

#ifndef MYWEB_POLL_H
#define MYWEB_POLL_H//IO 多路复用


#include "Epoll_event.h"
#include "Socket.h"
#include <memory>
#include <sys/epoll.h>
#include <vector>


class Socket;

class Epoll {
public:
    explicit Epoll() = default;

    ~Epoll() { ::close(m_epollfd); }

    void Create_fd() {
        m_epollfd = ::epoll_create1(EPOLL_CLOEXEC);
    }

    int Add_Channel(Epoll_event &&ev) const {
        return epoll_ctl(m_epollfd, EPOLL_CTL_ADD, ev.EventFd(), ev.Pointer());
    }

    int Remove_Channel(Epoll_event &&ev) const {
        return epoll_ctl(m_epollfd, EPOLL_CTL_DEL, ev.EventFd(), ev.Pointer());
    }

    int Update_Channel(Epoll_event &&ev) const {
        return epoll_ctl(m_epollfd, EPOLL_CTL_MOD, ev.EventFd(), ev.Pointer());
    }

    //尽可能的使用右值移动构造
    //设置一个epoll_wait
    int Wait(EpollEventResult &result) {
        //设置epoll_Wait超时
        int user_number = epoll_wait(m_epollfd, result.get(), result.m_store.size(), 4);
        return user_number;
    }

private:
    int m_epollfd;
};


#endif //MYWEB_POLL_H
