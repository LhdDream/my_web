//
// Created by kiosk on 19-8-2.
//
#include "Socket.h"
#include <iostream>
#include <string.h>
int  Socket::bindaddress()
{
    return ::bind(sockfd_,(struct sockaddr *)&address,sizeof(struct sockaddr));
}
int  Socket::listen()
{
    return ::listen(sockfd_,5);//默认backlog
}
void Socket::setresueport(bool on) {
    if (on == true) {
        int reuse = 1;
        ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(int));
        ::setsockopt(sockfd_,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));

    }
}
int Socket::accpet()
{
    //   ::accept()

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    return ::accept(sockfd_,(struct sockaddr *)&client,&client_addrlength);
}
void Socket::shutdownWrite()
{
    if( ::shutdown(sockfd_,SHUT_WR)< 0 )
    {
        std::cout <<  "shutdown error" << std::endl;
        return ;
    }
}
