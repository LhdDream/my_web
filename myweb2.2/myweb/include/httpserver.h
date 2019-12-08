//
// Created by kiosk on 19-8-5.
//

#ifndef MYWEB_HTTPSERVER_H
#define MYWEB_HTTPSERVER_H

#include "channel.h"
#include "poll.h"
#include "Acceptor.h"
#include <csignal>

class httpserver {
public:
    httpserver();

    void start();

private:
    std::unique_ptr<Acceptor> acceptor_; // 独占式的
    poll Epoll_; // epoll
    channel_set users_; // 所有用户

};

#endif //MYWEB_HTTPSERVER_H
