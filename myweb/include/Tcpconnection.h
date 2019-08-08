//
// Created by kiosk on 19-8-3.
//

#ifndef MYWEB_TCPCONNECTION_H
#define MYWEB_TCPCONNECTION_H
//connection 来进行链接对于每一个accpet的连接进行处理
#include <boost/any.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <Socket.h>
#include "channel.h"
#include "Eventloop.h"
#include <functional>
class Socket;
class channel;
class Eventloop;
//返回一个指向自身的enable_shared_from_this 指针
//tcpconnection 　管理所有的acceptor  socket
//
class Tcpconnection;
typedef  std::shared_ptr<Tcpconnection> Tcpptr;
typedef std::function<void(const Tcpptr &) > Messageback;
class Tcpconnection : public boost::enable_shared_from_this<Tcpconnection>
{
    public:
    // 连接来临的回调函数

    //初始化各种回调函数
    Tcpconnection(Eventloop * loop,int sockfd): loop_(loop),state_(Connecting),sockfd_(sockfd),channel_(new channel(loop,sockfd_))
    {
       channel_->handleRead(std::bind(&Tcpconnection::handleread,this));
    }
    void connectget();
    void handleread();
private:
    enum State {Connecting,Connected};
    // tcp状态机

    // 如果套接子可读触发相应的事件
     //  这里需要一个改变tcp 连接状态的函数
    void setState_(State s) { state_ = s ;}
   // 接受到http请求之后立马注册epollout 时间，如果可写就写入
    State  state_;
    Eventloop * loop_;
    std::unique_ptr<channel> channel_;
    int sockfd_;
};
//指向自身的shared_ptr 指针,tcpconnectionptr

#endif //MYWEB_TCPCONNECTION_H
