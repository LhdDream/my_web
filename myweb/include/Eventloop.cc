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
    while(true)
    {
        channellist.clear();
        poll_->run(&channellist);
        if (channellist.size() != 0) {
            for (auto &c : channellist) {
                c->handleEvent();
            }
        }
    }
}