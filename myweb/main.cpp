//#include <iostream>
#include "Buffer.h"
#include "channel.h"
#include "Eventloop.h"
#include "poll.h"
#include "Socket.h"
#include <signal.h>
#include "Tcpconnection.h"
#include "httpserver.h"
#include "httpresponse.h"
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