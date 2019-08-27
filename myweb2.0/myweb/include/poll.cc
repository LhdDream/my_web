#include "poll.h"
#include <sys/epoll.h>
#include <fcntl.h>
#include "Socket.h"
#include "iostream"


void poll::run(poll::channellist * activechannels) // 使用epoll 中的ptr 作为channlist
{
    auto num = epoll_wait(epollfd,&*events_.begin(),events_.size(),100);
    if(num > 0)
    {
        fillActiveChannels(num, activechannels); // 寻找活跃的连接
        if((size_t) num == events_.size() - 1)
        {
            events_.resize(events_.size() *2);
        }
    }
};
void poll::fillActiveChannels(int num,
                                   channellist* activeChannels)
{
    for(int i = 0 ; i < num ; i++)
    {
        events_[i].data.ptr = new channel(loop_.get(),events_[i].data.fd);
        auto channel_ = static_cast<channel * > (events_[i].data.ptr);
        channel_->set_events(events_[i].events);
        activeChannels->push_back(channel_);
    }
}
// 更新fd 操作，在epoll 之中 如果当前事件Io 分发没在poll 之中维护的话，添加到
//事件之中，
void poll::updateChannel( channel* channel_)
{
    // 判断channel_ 在不在维护的map 数组之中
    //如果没在的话，添加并且添加进入epoll之中,在的话，可以进行修改或者删除
        update(EPOLL_CTL_ADD,channel_);
        if(channel_->hasnoevent()) {
            update(EPOLL_CTL_DEL, channel_);
        }
}

void poll::update(int operation,channel * channel_)
{
    struct epoll_event event_;
    bzero(&event_,sizeof(event_));
    event_.events = channel_->get_events() ;
    event_.data.fd = channel_->fd();
    if(::epoll_ctl(epollfd,operation,event_.data.fd,&event_) < 0)
    {
        perror("epoll   \n");
    }
}