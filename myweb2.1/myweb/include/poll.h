//
// Created by kiosk on 19-7-30.
//

#ifndef MYWEB_POLL_H
#define MYWEB_POLL_H//IO 多路复用
#include <sys/epoll.h>
#include <channel.h>
#include <vector>
#include <map>
#include "Eventloop.h"
#include "channel.h"
#include "Socket.h"
//epoll 反应堆模型
//epoll 中的事件可读将IO事件分发出去进行事件处理
class Eventloop;
class channel;
class Socket;
class poll
{
    public:
            typedef std::vector<channel*> channellist;
            poll(Eventloop * loop) : epollfd(::epoll_create1(EPOLL_CLOEXEC)),lt(false),loop_(loop),events_(50){
                if(epollfd < 0)
                {
                    std::cout << "create epoll error" << std::endl;
                }
            };
            ~poll() {
                close(epollfd);
           }
            void run(channellist * activechannels);
            void fillActiveChannels(int num,
                            channellist* activeChannels) ;
    //有事件添加到epoll,没有事件则在epoll中删除
            void updateChannel(channel* channel_);
            void update(int operation,channel * channel_);
private:
        int epollfd;
        bool  lt;
        std::unique_ptr<Eventloop> loop_;
        std::vector<struct epoll_event> events_;
        std::map<int,channel *> channelmap_;
         // 设置一个map 绑定fa 和channel
};
#endif //MYWEB_POLL_H
