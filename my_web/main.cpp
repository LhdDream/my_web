#include "HttpServer.h"
#include "../util/threads.h"
int main()
{
    signal(SIGPIPE,SIG_IGN);
    Proxy l;
    l.wait();
    return 0;
}