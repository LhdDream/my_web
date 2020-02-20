#include "include/core/HttpServer.h"
#include "util/Threads.h"

int main() {
    signal(SIGPIPE, SIG_IGN);
    Proxy m_proxy;
    m_proxy.Wait();
    return 0;
}