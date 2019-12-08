//
// Created by kiosk on 19-8-3.
//
#include "Acceptor.h"

void Acceptor::listen() {
    listening_.store(true);
    acceptSocket_->listen();
}

int Acceptor::handleRead() //套接字可读的状态
{
    //ET使用accept来进行
    while (true) {
        int connfd = acceptSocket_->accpet();
        if (connfd > 0) {
            p(connfd);
        }
        if (connfd < 0) {
            //这里处理一下，服务器端如果文件描述符耗尽来进行的操作
            //来打开这里来进行处理
            if (errno == EMFILE) {
                ::close(idlefd_);
                idlefd_ = acceptSocket_->accpet();
                ::close(idlefd_);
                idlefd_ = ::open("/dev/null", O_WRONLY | O_CLOEXEC);
            } else
                break;
        }
    }
    return false;
}
