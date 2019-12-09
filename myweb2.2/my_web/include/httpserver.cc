//
// Created by kiosk on 19-8-5.
//
#include "httpserver.h"

httpserver::httpserver() :acceptor_(std::make_unique<Acceptor>()), Epoll_(std::make_shared<poll>()), users_(std::make_unique<channel_set>(Epoll_)) {
}

void httpserver::start() {
    signal(SIGPIPE, SIG_IGN);


    if (!acceptor_->listening()) {
        acceptor_->listen();
    }
    acceptor_->setCallback( [&] (int fd) {
        users_->add({  std::make_shared<channel> (fd)});
    });
    Epoll_->add_channel({acceptor_->fd_(), Readable()});
    EpollEventResult event_(1024);
    size_t user_number;
    while (true) {
        user_number = 0;
        users_->run(event_,&user_number);
        for (size_t i = 0; i < user_number; i++) {
            auto it = event_[i];
            if (it.event_fd() == acceptor_->fd_()) {
                acceptor_->handleRead();
            } else if (it.ck(EpollEventType::KReadble)) {
                users_->doRead(it.event_fd());
            } else if (it.ck(EpollEventType::KWriteable)) {
                users_->doWrite(it.event_fd());
            } else if (it.ck(EpollEventType::KClose)) {
                users_->remove(it.event_fd());
            }
        }
            //如果没有事件则去处理一下超时事件
    }
}

