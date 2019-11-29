#ifndef MYWEB_SOCKET_H
#define MYWEB_SOCKET_H
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/tcp.h>
class Socket // 使用RAII 重新封装Socket 对象
{
public:
    explicit Socket(const char * ip,const uint16_t port) {
        sockfd_ = socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,0);
        bzero(&address,sizeof(struct sockaddr_in));
        address.sin_family = AF_INET;
        inet_pton(AF_INET,ip,&address.sin_addr);
        address.sin_port = htons(port);
    }; // 显示转换
    explicit  Socket(int sockfd) : sockfd_(sockfd){}
    ~Socket() {close(sockfd_);}
    inline int fd() noexcept { return sockfd_;} // 返回已经绑定的socketfd
    size_t  bindaddress();
    size_t  listen() ;
    size_t accpet();
    //if listen error to return
    void setresueport(bool on); // 开启端口复用
    void shutdownWrite();
private:
    size_t sockfd_; //套接字
    struct sockaddr_in address;
};
#endif //MYWEB_SOCKET_H