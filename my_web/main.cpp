#include "HttpServer.h"
#include "../util/threads.h"
int main()
{
    chdir("/home/kiosk/picture/www/picture");
    signal(SIGPIPE,SIG_IGN);
    Proxy l;
    l.wait();
    return 0;
}