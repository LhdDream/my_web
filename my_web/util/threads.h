//
// Created by kiosk on 19-12-4.
//

#ifndef MYWEB_THREADS_H
#define MYWEB_THREADS_H
#include <vector>
#include <thread>
#include <utility>
#include "../include/httpserver.h"

thread_local httpserver Server;
class Proxy {
public:
    explicit Proxy() {
        thcont_.reserve(3);
        for(int i = 0 ; i < 3 ; i++)
        {
            thcont_.emplace_back( +[] () {Server.Start();});
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
