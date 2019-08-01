#include <iostream>
#include "Buffer.h"
#include "channel.h"
#include "Eventloop.h"
#include "poll.h"
#include "Socket.h"
#include <signal.h>
#include "threadpool.hpp"

int main()
{
    signal(SIGPIPE,SIG_IGN);
    Socket * one = new Socket("127.0.0.1",8080);
    one->setresueport(true);
    one->bindaddress();
    one->listen();
    poll poll_(one);
    Eventloop loop_(&poll_);
    loop_.loop();
    one->shutdownWrite();
    return 0;
}