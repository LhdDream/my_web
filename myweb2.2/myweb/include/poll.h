//
// Created by kiosk on 19-7-30.
//

#ifndef MYWEB_POLL_H
#define MYWEB_POLL_H//IO 多路复用
#include <sys/epoll.h>
#include <fcntl.h>
#include <set>
#include <map>
#include "Epoll_event.h"
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
            using channellist =  std::set<channel*> ;
            explicit poll() : epollfd_(::epoll_create1(EPOLL_CLOEXEC)){
            };
            int fd() const { return epollfd_;}
            int add_channel(Epoll_event & ev) const {
                return epoll_ctl(epollfd_,EPOLL_CTL_ADD,ev.event_fd(),ev.pointer());
            }
            int add_channel(Epoll_event && ev) const {
                return epoll_ctl(epollfd_,EPOLL_CTL_ADD,ev.event_fd(),ev.pointer());
            }
            //左值和右值,但是所有函数的形参都是左值
            int remove_channel(const Epoll_event & ev) const  {
                return epoll_ctl(epollfd_,EPOLL_CTL_DEL,ev.event_fd(),ev.pointer());
            }
            int remove_channel(Epoll_event && ev) const {
                return epoll_ctl(epollfd_,EPOLL_CTL_DEL,ev.event_fd(),ev.pointer());
            }
            int update_channel(const Epoll_event & ev) const {
                return epoll_ctl(epollfd_,EPOLL_CTL_MOD,ev.event_fd(),ev.pointer());
            }
            int update_channel(Epoll_event && ev) const{
                return epoll_ctl(epollfd_,EPOLL_CTL_MOD,ev.event_fd(),ev.pointer());
            }
            //尽可能的使用右值移动构造
            //设置一个epoll_wait
            void Wait(EpollEventResult & result , int timeout = 100){
                //设置epoll_Wait超时
                result.resize();
                size_t  user_number = epoll_wait(epollfd_, result.get(),result.capacity(), timeout );
            }
private:
        int epollfd_;
};
#endif //MYWEB_POLL_H
