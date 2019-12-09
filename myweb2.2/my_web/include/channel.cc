#include "channel.h"

int channel::fd_() const {
    return Socket_->fd();
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
void channel_set::add(const std::shared_ptr<channel> &user) { //copy
    table_.emplace(user->fd_(), user.get());
    epoll_->add_channel(user->get_event());
    user->onRead_(
            [this, user = user]() {
                auto it = user->handler_->RecvRequese( parse_, respon_);
                if (it == -1) {
                    user->type_ = EpollEventType::KClose;
                } else if (it == 2) {
                    user->type_ = Allof();
                }
            }
    );
    user->onWrite_(
            [this, user = user]() {
                auto it = user->handler_->SendResponse( respon_);
                if (it == 2) {
                    user->type_ = Allof();
                } else if (it == -1) {
                    user->type_ = EpollEventType::KClose;
                }
            }
    );
}


void channel_set::remove(channel *&user) {
    epoll_->remove_channel(user->get_event());
    table_.erase(user->fd_());
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
        //��epoll���޸�״̬
        epoll_->update_channel(table_[id]->get_event());
    }
}

void channel_set::remove(int id) {
    epoll_->remove_channel(table_[id]->get_event());
    table_.erase(id);
}

void channel_set::run(EpollEventResult &result, size_t *user_number) {
    epoll_->Wait(result, user_number);
    if (*user_number > 0) {
        if (static_cast<size_t > (*user_number) == result.fillsize_() - 1) {
            result.resize(*user_number);
        }
    }
}