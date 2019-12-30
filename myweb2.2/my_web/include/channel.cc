#include "channel.h"

//set ...............................
//if C++11
// user == left
// copy
// user  == rigth
// move
void  channel_set::add(int fd) { //copy
    epoll_->add_channel({fd,Readable()});
}

void channel_set::doRead(int id) {
    m_Readable_(id);
}

void channel_set::doWrite(int id) {
    m_Writeable_(id);
}

void channel_set::remove(int fd,EpollEventType type) {
    epoll_->remove_channel({fd,type});
}

