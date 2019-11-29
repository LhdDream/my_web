//
// Created by kiosk on 19-8-2.
//

#ifndef MYWEB_ACCEPTOR_H
#define MYWEB_ACCEPTOR_H

#include "Socket.h"
#include "channel.h"
#include "Eventloop.h"

class Eventloop;
class channel;
class Socket;
class Acceptor // socket accpet 连接的接口
{
public:
     using ConnCallback =  std::function< void(int sockfd) >;


private:

};// TCP 如果连接上之后进行回调通知使用者，内部类
#endif //MYWEB_ACCEPTOR_H
