#include "channel.h"


void User::handleRead() {
    if (type_ & Readable()) {
        Readcallback_();
    }
}

void User::handleWrite() {
    if (type_ & Writeable()) {
        Writecallback_();
    }
}

void User::onRead_(Callback &&re) {
    Readcallback_ = std::move(re);
}

void User::onWrite_(Callback &&wr) {
    Writecallback_ = std::move(wr);
}

//set ...............................
//if C++11
// user == left
// copy
// user  == rigth
// move
void  channel_set::add(int fd) { //copy
    Getoper(fd);
    std::cout << "Get" << std::endl;
}

void channel_set::add(int fd, const std::shared_ptr<User> &c) {
    table_[fd] = c;
    c->type_ = Basic();
    epoll_->add_channel({fd,c->type_});
}


void channel_set::doRead(int id) {
    table_[id]->handleRead();
}

void channel_set::doWrite(int id) {
    table_[id]->handleWrite();
}

void channel_set::loop(const std::vector<int> & namelist) {
    for(auto &&c : namelist){
        if ( table_[c]->type_ & EpollEventType::KReadble) {
                doRead(c);
            } else if (table_[c]->type_ & EpollEventType::KWriteable) {
                doWrite(c);
            }
        //这里无须检测EPOLLRDHUP事件,close之后会直接从epoll中删除
    }
}