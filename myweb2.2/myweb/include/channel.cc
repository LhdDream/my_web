#include "channel.h"

int channel::fd_() const {
    return Socket_->fd();
}

void channel::handleRead() {
    Read_ = false;
    if (toType(type_) & toType(Readable())) {
        Readcallback_();
        Read_ = true;
        set_type_(Writeable());
    }
    //写完发送完才可以关闭
}

void channel::handleWrite() {
    Write_ = false;
    if (toType(type_) & toType(Writeable())) {
        Writecallback_();
        Write_ = true;
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
int channel_set::add(channel *&user) { //copy
    table_.emplace(user->fd_(), user);
    epoll_.add_channel({user->fd_(), user->get_()});

    //设置回调函数
    user->onRead_([&user]() {
        user->handler_->RecvRequese(user->ReadBuffer_);;
    });
    user->onWrite_([&user]() {
        user->handler_->SendResponse(user->ReadBuffer_);;
    });
    return user->fd_();
}


void channel_set::remove(channel *&user) {
    epoll_.remove_channel({user->fd_(), user->get_()});
    table_.erase(user->fd_());
    close(user->fd_());
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
        //从epoll中修改状态
        epoll_.update_channel({table_[id]->fd_(), table_[id]->get_()});
    }
}
