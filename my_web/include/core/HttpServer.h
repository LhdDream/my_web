//
// Created by kiosk on 19-8-5.
//

#ifndef MYWEB_HTTPSERVER_H
#define MYWEB_HTTPSERVER_H

#include "net/Acceptor.h"
#include "net/Epoll.h"
#include "UserSet.h"
#include "../../config/Provider.h"
#include "../../util/Timer.h"
#include <csignal>
#include <thread>



class httpserver {
public:
    httpserver();

    ~httpserver() = default;

    void Start();

private:
    Acceptor m_acceptor; // 独占式的
    Epoll m_epoll;
    User_set m_users; // 所有用户
    Timer m_timer;
};

#endif //MYWEB_HTTPSERVER_H
