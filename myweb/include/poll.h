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
            typedef std::vector<struct epoll_event> EventList;
            poll(Socket *data_ ) :m_user_count(0),lt(false),epollfd(::epoll_create1(EPOLL_CLOEXEC)),tmp(data_),events_(50){
                int listenfd = 0;
                add_event(tmp->fd(&listenfd));
                if(epollfd < 0)
                {
                    std::cout << "create epoll error" << std::endl;
                }
            };
            ~poll() {
        close(epollfd);
           }
            void add_event(int fd);
            void del_events(int fd);
            int setnonblocking(int fd); // 设置到非阻塞
            void run(channellist * activechannels);
    void fillActiveChannels(int num,
                            channellist* activeChannels) ;
    private:
        int epollfd;
        bool  lt;
        int m_user_count;
        Socket *tmp;
        EventList events_;
};
#endif //MYWEB_POLL_H
