#ifndef MYWEB_THREADS_H
#define MYWEB_THREADS_H

#include <thread>
#include <utility>
#include <vector>

#include "../config/Provider.h"
#include "core/HttpServer.h"

thread_local httpserver Server;

class Proxy {
public:
    explicit Proxy() {
        signal(SIGINT, Wait);
        Provider::Get();
        chdir(Provider::Get().ServerWwwRoot().c_str());
        m_thread.reserve(Provider::Get().ThreadsNumber());
        for (int i = 0; i < Provider::Get().ThreadsNumber(); i++) {
            m_thread.emplace_back([]() { Server.Start(); });
        }
        Server.Start();
    }

    static void Wait(int signo) {
        Provider::Get().Quit();
        for (auto& i : m_thread) {
            i.join();
        }
    }

private:
    static std::vector<std::thread> m_thread;
};
std::vector<std::thread> Proxy::m_thread;
#endif  // MYWEB_THREADS_H
