//
// Created by kiosk on 19-8-5.
//
#include "core/HttpServer.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

httpserver::httpserver() : m_acceptor{}, m_epoll(), m_users(m_epoll) {
    m_acceptor.SetCallback([&](int fd) {
        m_users.AddUser(fd);
        m_epoll.Add_Channel({fd, Readable()});
        m_timer.AddTimer(fd);
    });
    m_timer.SetCallback([&](int fd) { m_users.Remove(fd); });
}

void httpserver::Start() {
    m_acceptor.m_acceptSocket.CreateFd(Provider::Get().ServerIp().c_str(), Provider::Get().ServerPort());
    m_acceptor.m_acceptSocket.SetReuseport(true);
    m_acceptor.m_acceptSocket.BindAddress();
    m_acceptor.Listen();
    m_epoll.Create_fd();
    m_epoll.Add_Channel({m_acceptor.Fd(), Listen()});
    EpollEventResult event_;
    while (!Provider::Get().GetQuit()) {
        auto user_number = m_epoll.Wait(event_);
        for (int i = 0; i < user_number; i++) {
            auto&& it = event_[i];
            if (it.EventFd() == m_acceptor.Fd()) {
                m_acceptor.HandleRead();
            } else {
                if (it.Pointer()->events & EpollEventType::KReadable) {
                    m_users.DoRead(it.EventFd(), m_timer);
                } else if (it.Pointer()->events & EpollEventType::KWriteable) {
                    m_users.DoWrite(it.EventFd(), m_timer);
                } else if (it.Pointer()->events & EpollEventType::KClose) {
                    m_users.Remove(it.EventFd());
                }
            }
        }
        //处理超时事件
        m_timer.RemoveTimer();
    }
    //如果没有事件则去处理一下超时事件
}

#pragma clang diagnostic pop