//
// Created by kiosk on 19-8-2.
//

#ifndef MYWEB_ACCEPTOR_H
#define MYWEB_ACCEPTOR_H

#include "Socket.h"
#include "channel.h"

class Eventloop;

class channel;

class Socket;

class Acceptor // socket accpet 连接的接口
{
    friend class httpserver;

public:
    using ConnCallback =  std::function<void(int sockfd)>;

    explicit Acceptor() : acceptSocket_(std::make_unique<Socket>("127.0.0.1", 8080)), listening_{false},
                          idlefd_(::open("/dev/null", O_RDONLY | O_CLOEXEC)) {
        acceptSocket_->setresueport(true);
        acceptSocket_->bindaddress();
    }

    void listen();//初始化listen
    bool listening() const { return listening_; }

    int handleRead();

    int fd_() const {
        return acceptSocket_->fd();
    }

private:
    //C++ 中根据对象顺序进行初始化
    std::unique_ptr<Socket> acceptSocket_;
    bool listening_;
    int idlefd_; // 处理套接字的
};// TCP 如果连接上之后进行回调通知使用者，内部类
#endif //MYWEB_ACCEPTOR_H
