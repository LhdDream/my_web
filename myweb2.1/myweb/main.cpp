#include "channel.h"
#include "Eventloop.h"
#include <signal.h>
#include "httpserver.h"
#include "HttpContext.h"

int main()
{
    signal(SIGPIPE,SIG_IGN);
    chdir("/home/kiosk/picture/www");
    Eventloop loop;
    httpserver server(&loop);
    server.start();
    loop.loop();
    return 0;
}