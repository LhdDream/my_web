//
// Created by kiosk on 19-8-2.
//
#include "Socket.h"

size_t Socket::BindAddress () {
    return ::bind(m_sockfd, (struct sockaddr *) &m_address, sizeof(struct sockaddr));
}

size_t Socket::Listen() {
    return ::listen(m_sockfd, 4096);//默认backlog
}

void Socket::SetResueport(bool on)  {
    if (on) {
        int reuse = 1;
        ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(int));
        ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
        ::setsockopt(m_sockfd, IPPROTO_TCP, TCP_NODELAY, &reuse, sizeof(int));
        //SO_REUSEADDR作用式对于多核cpu,允许在同一个 ip , port 上运行
        //内核会采用负载均衡的方式客户端的连接请求给某一个服务器
    }
}

size_t Socket::Accpet() {
    struct sockaddr_in client{};
    socklen_t client_addrlength = sizeof(client);
    int fd = ::accept4(m_sockfd, (struct sockaddr *) &client, &client_addrlength, SOCK_NONBLOCK | SOCK_CLOEXEC);
    return fd;
}

void Socket::shutdownWrite() {
    if (::shutdown(m_sockfd, SHUT_WR) < 0) {
        return;
    }
}

int Socket::Read(const std::unique_ptr<Buffer>& buffer, int length, int flags) const {
    if(length > buffer->WriteAble())
    {
        buffer->ReSize(length);
        length = buffer->WriteAble();
    }

    int read_bytes = ::recv(m_sockfd, buffer->BeginWrite() ,length,flags) ;
    if(read_bytes > 0 )
    {
        buffer->WriteOffest_Move(read_bytes);
    }
    return read_bytes;
}

int Socket::Write(const void *buffer, int length, int flags) const {
    return ::send(m_sockfd,buffer,length,flags);
}