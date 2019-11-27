//
// Created by kiosk on 19-8-3.
//
#include "Acceptor.h"
#include "Eventloop.h"
#include "Socket.h"
#include "functional"
#include <fcntl.h>
#include <cerrno>
#include "channel.h"
Acceptor::Acceptor(Eventloop * loop) :loop_(loop),acceptSockct_("127.0.0.1",8080),acceptchannel_(new channel(loop_.get(),acceptSockct_.fd())),listening_(false),idlefd_(::open("/dev/null",O_RDONLY | O_CLOEXEC))
{
    acceptSockct_.setresueport(true);
    acceptSockct_.bindaddress();
    //绑定一个构造函数
    //如果监听套接字可读
    acceptchannel_->handleRead(std::bind(&Acceptor::handleRead,this));
}
void Acceptor::listen()
{
    listening_.store(true);
    acceptSockct_.listen();//让套接字可读
    acceptchannel_->enable_ET();
}

void Acceptor::handleRead() //套接字可读的状态
{
    while(true) {
        int connfd = acceptSockct_.accpet();
        if (connfd >= 0) {
            if (NewCallback_) {
                NewCallback_(connfd); // 这里新连接传递给tcp Server 之中
                //在这里我们可以打开一个新的文件描述符号,如果连接过多，文件描述符
                //用来，准备一个空闲的描述符号，或者我们可以使用判断理不理想
            } else {
                close(connfd);
            }
        }
        if (connfd < 0) {
           //这里处理一下，服务器端如果文件描述符耗尽来进行的操作
            //来打开这里来进行处理
           if (errno == EMFILE) {
                ::close(idlefd_);
                idlefd_ = acceptSockct_.accpet();
                ::close(idlefd_);
                idlefd_ = ::open("/dev/null", O_WRONLY | O_CLOEXEC);
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
               break;
            }
        }
    }
}
