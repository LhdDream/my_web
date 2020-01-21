//
// Created by kiosk on 19-8-5.
//
#include "HttpServer.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

httpserver::httpserver() :
        m_acceptor{},
        m_Epoll(), m_users(m_Epoll) {
    m_acceptor.SetCallback([&](int fd) {
        m_users.AddUser(fd);
        m_Epoll.Add_Channel({fd, Readable()});
        m_timer.AddTimer(fd);
    });
    m_timer.SetCallback([&](int fd) {
        m_users.Remove(fd);
    });
}

void httpserver::Start() {
    m_acceptor.m_acceptSocket.CreateFd();
    m_acceptor.m_acceptSocket.SetResueport(true);
    m_acceptor.m_acceptSocket.BindAddress();
    m_acceptor.Listen();
    m_Epoll.Create_fd();
    m_Epoll.Add_Channel({m_acceptor.Fd(), Listen_()});
    EpollEventResult event_;
    while (true) {
        auto user_number = m_Epoll.Wait(event_);
        for (int i = 0; i < user_number; i++) {
            auto &&it = event_[i];
            if (it.EventFd() == m_acceptor.Fd()) {
                m_acceptor.HandleRead();
            } else {
                if (it.Pointer()->events & EpollEventType::KReadble) {
                    m_users.DoRead(it.EventFd(), m_timer);
                } else if (it.Pointer()->events & EpollEventType::KWriteable) {
                    m_users.DoWrite(it.EventFd(), m_timer);
                } else if (it.Pointer()->events & EpollEventType::KClose) {
                    m_users.Remove(it.EventFd());
                }
            }
            //这里无须检测EPOLLRDHUP事件,close之后会直接从epoll中删除
        }
        //处理超时事件
        m_timer.RemoveTimer();
    }
    //如果没有事件则去处理一下超时事件
}


#pragma clang diagnostic pop