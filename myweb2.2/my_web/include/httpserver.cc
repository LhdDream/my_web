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
    acceptor_->acceptSocket_->setresueport(true);
    acceptor_->acceptSocket_->bindaddress();
    acceptor_->listen();
    Epoll_->add_channel({acceptor_->fd_(),Basic()});
    EpollEventResult event_(4096);

    while(true){
        auto user_number = Epoll_->Wait(event_);
        for(int i = 0 ; i < user_number; i++)
        {
            auto && it = event_[i];
            if(it.event_fd() == acceptor_->fd_()){
                acceptor_->handleRead();
            }else if ( it.pointer()->events & EpollEventType::KReadble) {
                users_->doRead(it.event_fd());
            } else if (it.pointer()->events & EpollEventType::KWriteable) {
                users_->doWrite(it.event_fd());
            }else if(it.pointer()->events & EpollEventType::KClose){
                users_->remove(it.event_fd(), reinterpret_cast<EpollEventType &>(it.pointer()->events));
            }
            //这里无须检测EPOLLRDHUP事件,close之后会直接从epoll中删除
        }
    }
    //如果没有事件则去处理一下超时事件
}

