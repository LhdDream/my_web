//
// Created by kiosk on 19-8-5.
//
#include "httpserver.h"

httpserver::httpserver() :acceptor_(std::make_unique<Acceptor>()), Epoll_(std::make_shared<poll>()), users_(std::make_unique<channel_set>(Epoll_)) {
    acceptor_->setCallback( [&] (int fd) {
        users_->add(fd);
    });
}

void httpserver::start() {

    if (!acceptor_->listening()) {
        acceptor_->listen();
    }
    Epoll_->add_channel({acceptor_->fd_(),Basic()});
    EpollEventResult event_(4096);
    size_t user_number;
    while(true){
        user_number = 0;
        Epoll_->Wait(event_,&user_number);
        for(int i = 0 ; i < user_number; i++)
        {
            auto && it = event_[i];
            if(it.event_fd() == acceptor_->fd_()){
                acceptor_->handleRead();
            }else if ( it.pointer()->events & EpollEventType::KReadble) {
                users_->doRead(it.pointer()->data.fd);
            } else if (it.pointer()->events & EpollEventType::KWriteable) {
                users_->doWrite(it.pointer()->data.fd);
            }else if(it.pointer()->events & EpollEventType::KClose){
                users_->remove(it.pointer()->data.fd, reinterpret_cast<EpollEventType &>(it.pointer()->events));
            }
            //这里无须检测EPOLLRDHUP事件,close之后会直接从epoll中删除
        }
    }
    //如果没有事件则去处理一下超时事件
}

