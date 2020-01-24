//
// Created by kiosk on 2020/1/23.
//

#ifndef MY_WEB_FASTCGIHANDLER_H
#define MY_WEB_FASTCGIHANDLER_H

#include "../config/provider.h"
#include "Currency.h"
#include "../include/Socket.h"
#include <map>

class HTTPMessage;
//与php-fpm进行连接，解析php ->> html 然后进行send发送
//每一个线程一个fastcgiHandler进行解析 或者每一个线程统一拥有一个
class FastCgiHandler{
public:
    explicit  FastCgiHandler() {
        m_FastCgi_Socket.CreateFd(Provider::Get().FastCgiIP().c_str(),Provider::Get().FastCgiPort());

    }
    ~FastCgiHandler()  = default;
    void SendFastCgi(const std::unique_ptr<HTTPMessage> &conn);
    void ReadFromPHP(Socket &  sockfd);
private:
    Socket m_FastCgi_Socket;
    Buffer m_buffer;
};
#endif //MY_WEB_FASTCGIHANDLER_H
