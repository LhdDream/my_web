//
// Created by kiosk on 19-8-5.
//

#ifndef MYWEB_HTTPSERVER_H
#define MYWEB_HTTPSERVER_H

#include "channel.h"
#include "poll.h"
#include "Acceptor.h"
#include <csignal>
#include <thread>

class httpserver  {
public:
    httpserver();

    void Start();

private:
    Acceptor m_acceptor; // 独占式的
    poll m_Epoll;
    User_set m_users; // 所有用户
};

#endif //MYWEB_HTTPSERVER_H
