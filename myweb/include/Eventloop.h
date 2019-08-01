//
// Created by kiosk on 19-7-30.
//

#ifndef MYWEB_EVENTLOOP_H
#define MYWEB_EVENTLOOP_H
#include <map>
#include <atomic>
#include <functional>
#include <vector>
#include <boost/any.hpp>
#include <thread>
#include "channel.h"
#include "poll.h"
#include <boost/scoped_ptr.hpp>
class channel;
class poll;
class Eventloop
{
public:

    Eventloop(poll * polp);
    ~Eventloop();
    void loop() ; //事件循环

private:

    std::atomic<bool> looping_;
    std::vector<channel *> channellist;
    poll* poll_;
};
#endif //MYWEB_EVENTLOOP_H
