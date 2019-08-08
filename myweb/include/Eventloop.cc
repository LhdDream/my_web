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
Eventloop::Eventloop() : channel_fd(createEventfd()),looping_(false),poll_(new poll(this)),channel_(new channel(this,channel_fd)),threadid_(gettid())
{


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
         if(channellist.size() != 0) {
            for (auto &c : channellist) {
                c->handleEvent();
            }
        }
        dofucting();
    }
}
void Eventloop::update(channel *channel_)

{ poll_->updateChannel(channel_);}

void Eventloop::getSocket( int connfd )
{  //std::cout << "socket   " << data->fd() << std::endl;
    poll_->getSocket(connfd);
}
void Eventloop::dofucting()
{
    // 这里使用swap 直接进行交换，让其他进行同时也可以在其中添加事件
    std::vector<Funct> fucnting_;
    {
        std::lock_guard<std::mutex> te(mutex_);
        fucnting_.swap(funting);
    }
    for(size_t i = 0 ; i  < fucnting_.size() ; i++)
    {
        fucnting_[i]();
    }
}