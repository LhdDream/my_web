#include "Eventloop.h"
#include <mutex>
#include <iostream>
#include <sys/eventfd.h>
static int createEventfd()
{
    int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    //eventfd  有八个字节
    //reactor 经常会阻塞到epoll_wait 需要唤醒
    if (evtfd < 0)
    {
        abort();
    }
    return evtfd;
}
//weakfd 的作用　，　唤醒多线程中的沉睡的线程
Eventloop::Eventloop() :looping_(false), poll_(new poll(this)),channel_fd(createEventfd()),channel_(new channel(this,channel_fd)),threadid_(gettid())
{
    channel_->handleRead(std::bind(&Eventloop::handleRead,this));
    channel_->enable_ET();
}

Eventloop::~Eventloop()
{

}
void Eventloop::loop(void )
{
    while(true)
    {
        channellist.clear();
        poll_->run(&channellist);
        for (auto &c : channellist) {
            c->handleEvent();
        }
        dofucting();
    }
}
void Eventloop::update(channel *channels)
{
    poll_->updateChannel(channels);
}

void Eventloop::dofucting()
{
    // 这里使用swap 直接进行交换，让其他进行同时也可以在其中添加事件
    looping_.store(true);
    std::vector<Funct> fucnting_;
    {
        std::lock_guard<std::mutex> te(mutex_);
        fucnting_.swap(funting);
    }
    for(const auto & functor : fucnting_)
    {
        functor();
    }
    looping_.store(false);
}
void Eventloop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = read(channel_fd, &one, sizeof one);
    if (n != sizeof one)
    {
       printf("error read");
    }
}