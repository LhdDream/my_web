//
// Created by kiosk on 19-8-2.
//
#include "Socket.h"
#include <iostream>
#include <string.h>
#include <netinet/tcp.h>
int  Socket::bindaddress()
{
    return ::bind(sockfd_,(struct sockaddr *)&address,sizeof(struct sockaddr));
}
int  Socket::listen()
{
    return ::listen(sockfd_,10024);//默认backlog
}
void Socket::setresueport(bool on) {
    if (on == true) {
        int reuse = 1;
        ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(int));
        ::setsockopt(sockfd_,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
        ::setsockopt(sockfd_,IPPROTO_TCP,TCP_NODELAY,&reuse,sizeof(int));
        ::setsockopt(sockfd_,SOL_SOCKET,SO_KEEPALIVE,&reuse,sizeof(int));
        reuse = 12;
        //每超过9秒进行一次发包
        ::setsockopt(sockfd_,IPPROTO_TCP,TCP_KEEPIDLE,&reuse,sizeof(int));
        reuse = 4;
        ::setsockopt(sockfd_,IPPROTO_TCP,TCP_KEEPINTVL,&reuse, sizeof(int));
        reuse = 3;
        ::setsockopt(sockfd_,IPPROTO_TCP,TCP_KEEPCNT,&reuse,sizeof(int));
        //SO_REUSEADDR作用式对于多核cpu,允许在同一个 ip , port 上运行
        //内核会采用负载均衡的方式客户端的连接请求给某一个服务器
    }
}
int Socket::accpet()
{
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int fd = ::accept4(sockfd_,(struct sockaddr *)&client,&client_addrlength,SOCK_NONBLOCK  | SOCK_CLOEXEC);
    return fd;
}
void Socket::shutdownWrite()
{
    if( ::shutdown(sockfd_,SHUT_WR)< 0 )
    {
        std::cout <<  "shutdown error" << std::endl;
        return ;
    }
}
