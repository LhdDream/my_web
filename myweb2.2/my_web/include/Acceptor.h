//
// Created by kiosk on 19-8-2.
//

#ifndef MYWEB_ACCEPTOR_H
#define MYWEB_ACCEPTOR_H

#include "Socket.h"
#include "channel.h"
#include <atomic>
#include <functional>


class Acceptor // socket accpet 连接的接口
{
    friend  class httpserver;
public:
    using ConnCallback =  std::function<void(int sockfd)>;

    explicit Acceptor() : acceptSocket_(nullptr),
                          idlefd_(::open("/dev/null", O_RDONLY | O_CLOEXEC)) {
        createSocket_();
    }
    void createSocket_(const char * ip = "127.0.0.1",uint16_t port =8080)
    {
        acceptSocket_ = std::make_unique<Socket>(ip,port);

    }
    void listen();//初始化listen


    void handleRead();

    int fd_() const {
        return acceptSocket_->fd();
    }
    void setCallback(ConnCallback && cb){
            p = std::move(cb);
    }
private:
    //C++ 中根据对象顺序进行初始化
    std::unique_ptr<Socket> acceptSocket_;
    int idlefd_; // 处理套接字的
    ConnCallback  p;
};// TCP 如果连接上之后进行回调通知使用者，内部类
#endif //MYWEB_ACCEPTOR_H
