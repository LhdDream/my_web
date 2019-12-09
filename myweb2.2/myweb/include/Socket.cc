//
// Created by kiosk on 19-8-2.
//
#include "Socket.h"

size_t Socket::bindaddress() {
    return ::bind(sockfd_, (struct sockaddr *) &address, sizeof(struct sockaddr));
}

size_t Socket::listen() {
    return ::listen(sockfd_, SOMAXCONN);//默认backlog
}

void Socket::setresueport(bool on) {
    if (on) {
        int reuse = 1;
        ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(int));
        ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
        ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &reuse, sizeof(int));
        ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, &reuse, sizeof(int));
        //SO_REUSEADDR作用式对于多核cpu,允许在同一个 ip , port 上运行
        //内核会采用负载均衡的方式客户端的连接请求给某一个服务器
    }
}

size_t Socket::accpet() {
    struct sockaddr_in client{};
    socklen_t client_addrlength = sizeof(client);
    int fd = ::accept4(sockfd_, (struct sockaddr *) &client, &client_addrlength, SOCK_NONBLOCK | SOCK_CLOEXEC);
    int reuse = 1;
    ::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &reuse, sizeof(int));
    return fd;
}

void Socket::shutdownWrite() {
    if (::shutdown(sockfd_, SHUT_WR) < 0) {
        return;
    }
}

int Socket::read(const std::shared_ptr<Buffer>& buffer, int length, int flags) const {
    if(length > buffer->writeable())
    {
        length = buffer->writeable();
    }

    int read_bytes = ::recv(sockfd_, buffer->beginwrite() ,length,flags) ;
    if(read_bytes > 0 )
    {
        buffer->writeoffest_move(read_bytes);
    }
    return read_bytes;
}

int Socket::write(const void *buffer, int length, int flags) const {
    return ::send(sockfd_,buffer,length,flags);
}