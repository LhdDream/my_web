#ifndef MYWEB_SOCKET_H
#define MYWEB_SOCKET_H

#include "core/Buffer.h"
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

class Socket // 使用RAII 重新封装Socket 对象
{
public:
    explicit Socket() {

    }; // 显示转换
    Socket(int sockfd) : m_sockfd(sockfd) {}

    ~Socket() { close(m_sockfd); }

    void CreateFd(const char *ip = "127.0.0.1", const int port = 8080) {
        m_sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
        bzero(&m_address, sizeof(struct sockaddr_in));
        m_address.sin_family = AF_INET;
        m_address.sin_addr.s_addr = INADDR_ANY;
        m_address.sin_port = htons(port);
    }

    int Fd() const { return m_sockfd; } // 返回已经绑定的socketfd
    size_t BindAddress();

    size_t Listen();

    size_t Accept();

    //if listen error to return
    void SetReuseport(bool on); // 开启端口复用


    int Read( Buffer &buffer, int length, int flags = 0) const;

    int Write(const void *buffer, int length, int flags = 0) const;

    int  Connect();
private:
    size_t m_sockfd; //套接字
    struct sockaddr_in m_address;
};

#endif //MYWEB_SOCKET_H