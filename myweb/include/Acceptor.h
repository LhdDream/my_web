//
// Created by kiosk on 19-8-2.
//

#ifndef MYWEB_ACCEPTOR_H
#define MYWEB_ACCEPTOR_H

#include "Socket.h"
#include "channel.h"
#include "Eventloop.h"
#include <functional>
class Eventloop;
class channel;
class Socket;
class Acceptor // socket accpet 连接的接口
{
public:
    typedef std::function< void(int sockfd)> ConnCallback;
     Acceptor(Eventloop* loop); //显示构造
     void setCallback(const ConnCallback & new_)//新连接来临选择回调函数
     {
        NewCallback_ = new_;
     }
     void listen(); // 初始化注册listen
     //之后把listen 套接字保存提交到epoll 中，然后通过channel 来进行可连接事件
     bool listening() const { return listening_;}
     void handleRead();//可写时间
private:
    Eventloop * loop_; //event loop acceptor 必须在loop之中
                        // 包括一些channel 循环, poll
                        //单步依赖
    Socket acceptSockct_;
    channel acceptchannel_;
    ConnCallback NewCallback_;
    bool listening_ ;
    int idlefd_;
};// TCP 如果连接上之后进行回调通知使用者，内部类
#endif //MYWEB_ACCEPTOR_H
