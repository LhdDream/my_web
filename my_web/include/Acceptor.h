//
// Created by kiosk on 19-8-2.
//

#ifndef MYWEB_ACCEPTOR_H
#define MYWEB_ACCEPTOR_H

#include <fcntl.h>
#include <functional>
#include "Socket.h"
#include <sys/types.h>
#include <sys/stat.h>


class Acceptor // socket accpet 连接的接口
{
    friend class httpserver;

public:
    using ConnCallback =  std::function<void(int sockfd)>;

    explicit Acceptor() : m_acceptSocket(), m_idlefd(::open("/dev/null", O_RDONLY | O_CLOEXEC)) {


    }

    void Listen();//初始化listen


    void HandleRead();

    int Fd() const {
        return m_acceptSocket.Fd();
    }

    void SetCallback(ConnCallback &&cb) {
        p = std::move(cb);
    }

private:
    //C++ 中根据对象顺序进行初始化
    Socket m_acceptSocket;
    int m_idlefd; // 处理套接字的
    ConnCallback p;
};// TCP 如果连接上之后进行回调通知使用者，内部类
#endif //MYWEB_ACCEPTOR_H
