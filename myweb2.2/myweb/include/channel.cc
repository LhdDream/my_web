#include "channel.h"
size_t channel::fd() const {
    return Socket_->fd();
}

void channel::handleEvent() {

}