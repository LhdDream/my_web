//
// Created by kiosk on 19-8-5.
//
#include "httpserver.h"

httpserver::httpserver() :acceptor_(std::make_unique<Acceptor>()), Epoll_(std::make_shared<poll>()), users_(std::make_unique<channel_set>(Epoll_)) {
}

void httpserver::start() {

    if (!acceptor_->listening()) {
        acceptor_->listen();
    }
    acceptor_->setCallback( [&] (int fd) {
        users_->add(fd);
    });
    EpollEventResult event_(1024);
    Epoll_->add_channel({acceptor_->fd_(), Basic()});
    size_t user_number;
    while(true){
        activeFd.clear();
        user_number = 0;
        Epoll_->Wait(event_,&user_number);
        for(int i = 0 ; i < user_number; i++)
        {
            auto &&it = event_[i];
            if(it.event_fd() == acceptor_->fd_()){
                acceptor_->handleRead();
            }else {
                activeFd.emplace_back(it.event_fd());
            }
        }
        users_->loop(activeFd);
    }
    //如果没有事件则去处理一下超时事件
}

