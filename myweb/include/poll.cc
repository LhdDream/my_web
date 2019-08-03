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
    event.events = EPOLLIN  | EPOLLET | EPOLLRDHUP;
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

    for(int i = 0 ; i < num ; i++)
    {
        int sockfd = events_[i].data.fd;
        int listenfd = 0;
        channel * channel_ =  new channel(events_[i].data.fd);
        //lib 反应堆
        if(sockfd == tmp->fd(&listenfd))
        {
            int connfd = tmp->accpet();
            add_event(sockfd);
            m_user_count++;
            channel_->set_fd(connfd);
        }
        if(events_[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
        {
            close(sockfd);
            m_user_count--;
            del_events(sockfd);
        }
        channel_->set_events(events_[i].events);
        activeChannels->push_back(channel_);
    }

}

