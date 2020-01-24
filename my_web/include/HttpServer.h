//
// Created by kiosk on 19-8-5.
//

#ifndef MYWEB_HTTPSERVER_H
#define MYWEB_HTTPSERVER_H


#include "Acceptor.h"
#include "../config/provider.h"
#include <csignal>
#include "Epoll.h"
#include "UserSet.h"
#include "../util/timer.h"
#include <thread>

class httpserver {
public:
    httpserver();

    ~httpserver() = default;

    void Start();

private:
    Acceptor m_acceptor; // 独占式的
    Epoll m_Epoll;
    User_set m_users; // 所有用户
    Timer m_timer;
};

#endif //MYWEB_HTTPSERVER_H
