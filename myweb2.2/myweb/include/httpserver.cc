//
// Created by kiosk on 19-8-5.
//
#include "httpserver.h"

httpserver::httpserver() :poll_(std::make_unique<fixed_thread_pool>(1)),
acceptor_(std::make_unique<Acceptor>()),Epoll_(),users_(Epoll_)
{
    acceptor_->setCallback([&](int sockfd)  {
           auto  p = new channel(sockfd);
           users_.add(p);
           poll_->execute([&] () {
                users_.doall(sockfd);
                users_.closechannel(sockfd);
           });
    });
}

void httpserver::start() {
    signal(SIGPIPE,SIG_IGN);
    if(!acceptor_->listening())
    {
        acceptor_->listen();
    }
    Epoll_.add_channel({acceptor_->fd_(),Readable()});
    EpollEventResult event_(1024);

    while (true){
        auto user_number = Epoll_.Wait(event_);
        for(int i = 0 ; i < user_number ; i++){
                auto it  = event_[i];
                if(it.event_fd() == acceptor_->fd_())
                {
                    acceptor_->acceptchannel_->handleEvent_();
                }
                //如果没有事件则去处理一下超时事件
        }
    }
}

