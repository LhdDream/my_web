#include "core/HttpServer.h"
#include "Threads.h"
int main()
{
    signal(SIGPIPE,SIG_IGN);
    Proxy l;
    l.wait();
    return 0;
}