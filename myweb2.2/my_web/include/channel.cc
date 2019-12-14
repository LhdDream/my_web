#include "channel.h"

int channel::fd_() const {
    return Socket_;
}

void channel::handleRead() {
    if (type_ & Readable()) {
        Readcallback_();
    }
}

void channel::handleWrite() {
    if (type_ & Writeable()) {
        Writecallback_();
    }

}

void channel::onRead_(Callback &&re) {
    Readcallback_ = std::move(re);
}

void channel::onWrite_(Callback &&wr) {
    Writecallback_ = std::move(wr);
}

//set ...............................
//if C++11
// user == left
// copy
// user  == rigth
// move
void channel_set::add(int fd) { //copy
    Getoper(fd);
}


void channel_set::remove(const std::shared_ptr<channel> & user) {
    epoll_->remove_channel(user->get_event());
}

void channel_set::doRead(int id) {
    table_[id]->handleRead();
    closechannel(id);
}

void channel_set::doWrite(int id) {
    table_[id]->handleWrite();
    closechannel(id);
}


void channel_set::closechannel(int id) {

    if (table_[id]->closeable()) {
        remove(table_[id]);
    } else {
        epoll_->update_channel(table_[id]->get_event());
    }
}

void channel_set::remove(int id) {
    epoll_->remove_channel(table_[id]->get_event());
}

void channel_set::run(EpollEventResult &result, size_t *user_number) {
    epoll_->Wait(result, user_number);
    if (*user_number > 0) {
        if (static_cast<size_t > (*user_number) == result.fillsize_() - 1) {
            result.resize(*user_number);
        }
    }
}