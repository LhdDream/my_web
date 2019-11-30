#include "channel.h"
int channel::fd_() const {
    return Socket_->fd();
}

void channel::handleEvent_() {
    Read_ = false;
    Write_ = false;
    if(toType(type_) & toType(Readable()))
    {
        Readcallback_();
        Read_= true;
    }
    else if(toType(type_) & toType(Writeable()))
    {
        Writecallback_();
        Write_ = true;
    }
    //写完发送完才可以关闭
}

void channel::onRead_(Callback && re) {
    Readcallback_ = std::move(re);
}

void channel::onWrite_(Callback && wr) {
    Writecallback_ = std::move(wr);
}

//set ...............................
//if C++11
// user == left
// copy
// user  == rigth
// move
int channel_set::add(channel * &user) { //copy
    table_.emplace(user->fd_(),user);
    epoll_.add_channel({user->fd_(),user->get_()});

    //设置回调函数
    user->onRead_([&user]() {
        user->handler_->RecvRequese(user->ReadBuffer_);
        user->set_type_(Writeable());
        ;});
    user->onWrite_( [&user] ()  {
        user->handler_->SendResponse(user->ReadBuffer_);
        user->set_type_(Readable());
        ;});
    user->set_type_(Readable());
    return user->fd_();
}
int channel_set::add(channel *&&user) { // move
    table_.emplace(user->fd_(),user);
    epoll_.add_channel({user->fd_(),user->get_()});


    //设置回调函数
    user->onRead_([&user]() {
        user->handler_->RecvRequese(user->ReadBuffer_);
        user->set_type_(Writeable());
        ;});
    user->onWrite_( [&user] () {
        user->handler_->SendResponse(user->ReadBuffer_);
        user->set_type_(Readable());
        ;});
    user->set_type_(Readable());
    return user->fd_();
}


void channel_set::remove(channel *&user) {
    epoll_.remove_channel({user->fd_(),user->get_()});
    table_.erase(user->fd_());
    close(user->fd_());
}

void channel_set::modify(channel *&user) {
    if(table_.find(user->fd_()) != table_.end())
    {
        epoll_.update_channel({user->fd_(),user->get_()});
    }
}



void channel_set::doall(int id) {
    table_[id]->handleEvent_();
}

void channel_set::closechannel(int id) {
    if(table_[id]->closeable())
    {
        remove(table_[id]);
    }else
    {
        //从epoll中修改状态
        epoll_.update_channel({table_[id]->fd_(),table_[id]->get_()});
    }
}
