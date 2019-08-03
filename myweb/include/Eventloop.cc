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
        if (channellist.size() != 0) {
            for (auto &c : channellist) {
                c->handleEvent();
            }
        }
        channellist.clear(); // 清除
    }
}