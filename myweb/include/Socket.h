#ifndef MYWEB_SOCKET_H
#define MYWEB_SOCKET_H
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "iostream"
class Socket // 使用RAII 重新封装Socket 对象
{
public:
    explicit Socket(const char * ip,const uint16_t port) {
        sockfd_ = socket(PF_INET,SOCK_STREAM,0);
        bzero(&address,sizeof(struct sockaddr_in));
        address.sin_family = AF_INET;
        inet_pton(AF_INET,ip,&address.sin_addr);
        address.sin_port = htons(port);
    };
    int fd() { return sockfd_;} // 返回已经绑定的socketfd
    int  bindaddress();
    //abort if bind in use
    int  listen();
    //if listen error to return
    void setresueport(bool on); // 开启端口复用
    int accpet();
    void shutdownWrite();
    ~Socket(){  std::cout <<" 析够 " << close(sockfd_);};
private:
    int  sockfd_; //套接字
    struct sockaddr_in address;
};
#endif //MYWEB_SOCKET_H