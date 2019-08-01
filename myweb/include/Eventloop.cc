#include "Eventloop.h"
#include <mutex>
#include <iostream>
std::mutex mutex_;
Eventloop::Eventloop(poll * polp) : looping_(false),poll_(polp)
{

}

Eventloop::~Eventloop()
{

}
void Eventloop::loop()
{

    channellist.clear();

    poll_->run(&channellist);

    while(true) {
       // std::unique_lock<std::mutex> tmp(mutex_);
       // tmp.lock();
        if (channellist.size() == 0) {
           // std::cout << "size == 0 strerror((errno)) " << std::endl;
        }
        for (auto &c : channellist) {
         c->handleEvent();
        }
     //   tmp.unlock();
    }
}