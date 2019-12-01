//
// Created by kiosk on 19-8-5.
//
#include "httpserver.h"

httpserver::httpserver() : poll_(std::make_unique<fixed_thread_pool>(0)),acceptor_(std::make_unique<Acceptor>()), Epoll_(), users_(Epoll_) {
}

void httpserver::start() {
    signal(SIGPIPE, SIG_IGN);
    if (!acceptor_->listening()) {
        acceptor_->listen();
    }
    Epoll_.add_channel({acceptor_->fd_(), Readable()});
    EpollEventResult event_(1024);
    size_t user_number;
    while (true) {
        user_number = 0;
        Epoll_.Wait(event_, &user_number);
        for (size_t i = 0; i < user_number; i++) {
            auto it = event_[i];
            if (it.event_fd() == acceptor_->fd_()) {
                int client_fd = acceptor_->handleRead();
                auto clien = new channel(client_fd);
                users_.add(clien);
            } else if (it.ck(EpollEventType::KReadble)) {
                users_.doRead(it.event_fd());
            } else if (it.ck(EpollEventType::KWriteable)) {
                users_.doWrite(it.event_fd());
            }
            //如果没有事件则去处理一下超时事件
        }
    }
}

