#include "poll.h"
#include <sys/epoll.h>
#include <fcntl.h>
#include "Socket.h"
#include "iostream"
int poll::setnonblocking(int fd) {
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}
void poll::add_event(int fd)
{

    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN  | EPOLLPRI ;
    if( lt )
    {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    setnonblocking(fd);
}
void poll::del_events(int fd)
{
    epoll_event event;
    bzero(&event,sizeof(event));
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&event);
};
void poll::run(poll::channellist * activechannels) // 使用epoll 中的ptr 作为channlist
{
    loop_->getSocket(loop_->get());
    add_event(sockfd_);
    int num = epoll_wait(epollfd,events_.data(),events_.size(),-1);
    if(num > 0)
    {
        fillActiveChannels(num, activechannels); // 寻找活跃的连接
        if((size_t) num == events_.size())
        {
            events_.resize(events_.size() *2);
        }
    }
};
void poll::fillActiveChannels(int num,
                                   channellist* activeChannels)
{
   // std::cout << "Chann " << std::endl;
    for(int i = 0 ; i < num ; i++)
    {
        channel * channel_ = channelmap_[events_[i].data.fd];
        channel_->set_events(events_[i].events);
        activeChannels->push_back(channel_);
        channelmap_[events_[i].data.fd] = channel_;
    }
}
// 更新fd 操作，在epoll 之中 如果当前事件Io 分发没在poll 之中维护的话，添加到
//事件之中，
void poll::updateChannel(channel* channel_)
{
    // 判断channel_ 在不在维护的map 数组之中
    //如果没在的话，添加并且添加进入epoll之中,在的话，可以进行修改或者删除
  //  std::cout << "poll::updateChannel ";
   // std::cout << channel_->fd() << std::endl;
    int fd = channel_->fd();
    if(channelmap_.find(fd) == channelmap_.end())
    {
        add_event(fd);
        channelmap_[channel_->fd()] = channel_;
    }
    else if(channelmap_.find(fd) != channelmap_.end())
    {
        if(channel_->hasnoevent()) {
            del_events(channel_->fd());
            channelmap_.erase(channel_->fd());
        }
        channelmap_[channel_->fd()] = channel_;
    }

}