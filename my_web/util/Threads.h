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
        thcont_.reserve(Provider::Get().ThreadsNumber());
        for (int i = 0; i < Provider::Get().ThreadsNumber(); i++) {
            thcont_.emplace_back([]() { Server.Start(); });
        }
        Server.Start();
    }

    int wait() {
        for (auto &i: thcont_) {
            i.join();
        }

        return 0;
    }

private:
    std::vector<std::thread> thcont_;
};

#endif //MYWEB_THREADS_H
