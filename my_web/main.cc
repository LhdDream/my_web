#include "include/core/HttpServer.h"
#include "util/Threads.h"
int main()
{
    signal(SIGPIPE,SIG_IGN);
    Proxy l;
    l.wait();
    return 0;
}