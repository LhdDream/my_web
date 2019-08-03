//
// Created by kiosk on 19-8-2.
//

#ifndef MYWEB_ACCEPTOR_H
#define MYWEB_ACCEPTOR_H

#include "Socket.h"
#include "channel.h"
class Acceptor // socket accpet 连接的接口
{
public:
     Acceptor(Eventloop * loop);
     void setCallback()//新连接来临选择回调函数
     {

     }

private:
    void handleRead() ; // 有可写事件
    Eventloop * loop_;
    Socket acceptSockct_;
    channel acceptchannel_;

};// TCP 如果连接上之后进行回调通知使用者，内部类
#endif //MYWEB_ACCEPTOR_H
