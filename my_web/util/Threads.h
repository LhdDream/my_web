//
// Created by kiosk on 19-12-4.
//

#ifndef MYWEB_THREADS_H
#define MYWEB_THREADS_H

#include "core/HttpServer.h"
#include "../config/Provider.h"
#include <vector>
#include <thread>
#include <utility>

thread_local httpserver Server;

class Proxy {
public:
    explicit Proxy() {
        Provider::Get();
        chdir(Provider::Get().ServerWwwRoot().c_str());
        m_thread.reserve(Provider::Get().ThreadsNumber());
        for (int i = 0; i < Provider::Get().ThreadsNumber(); i++) {
            m_thread.emplace_back([]() { Server.Start(); });
        }
        Server.Start();
    }

    void  Wait() {
        for (auto &i: m_thread) {
            i.join();
        }
    }

private:
    std::vector<std::thread> m_thread;
};

#endif //MYWEB_THREADS_H
