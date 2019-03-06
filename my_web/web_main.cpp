#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <signal.h>
#include "locker.hpp"

#include "threadpool.hpp"
#include "web_conn.hpp"
#include "webConn.cpp"
using namespace std;
#define maxfd 100
#define max_event_number 1000
void Handler(int iSignNo)
{
    ;
}

int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}
void addfd(int epfd, int fd, bool flag)
{
    epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    if (flag)
    {
        ev.events = ev.events | EPOLLONESHOT;
    }
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    setnonblocking(fd);
}
int main(int argc, char **argv)
{
   
    const char* ip = "127.0.0.1";
    int port = 8888;//port端口
    // const char * ip =argv[1];
    // int port =atoi(argv[2]);
  


    signal(SIGPIPE, Handler);

    threadpool<web_conn> * pool = NULL;

    pool = new threadpool<web_conn>;
    web_conn * member = new web_conn[maxfd];

    int user_count = 0;//客户数量

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);

    struct linger tmp = {1,0};
    setsockopt(listenfd,SOL_SOCKET,SO_LINGER,&tmp,sizeof(tmp));

    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);
 

    int ret = bind(listenfd,(struct sockaddr *)&address,sizeof(address));
    assert(ret >= 0);

    ret = listen(listenfd,5);
    assert(ret >= 0);

    int epfd;
    epoll_event events [max_event_number];
    epfd = epoll_create(5);
    assert(epfd != -1);
  
    addfd(epfd,listenfd,false);

    while (true)
    {
        int number = epoll_wait(epfd, events, 1000, -1);
     
        if ((number < 0) && (errno != EINTR))
        {
            cout <<__LINE__ << endl;
            break;
        }
        for (int i = 0; i < number; i++)
        {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd) //有新用户连接
            {
                struct sockaddr_in client_address;
                socklen_t client_addresslength = sizeof(client_address);
                int client_fd = accept(listenfd, (struct sockaddr *)&client_address, &client_addresslength);
                if (client_fd < 0)
                {
                    printf("errno is %d\n", errno);
                    continue;
                }
                addfd(epfd, client_fd, true);
                member[client_fd].web_init(client_fd, client_address,epfd);
            }
            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                /*出现异常则关闭客户端连接*/
                member[sockfd].close_conn();
            }
            else if (events[i].events & EPOLLIN) //可以读取
            {
               
                if (member[sockfd].read_file())
                {

                    /*读取成功则添加任务队列*/
                    pool->add_thread(member + sockfd);
                }
                else
                {
                    member[sockfd].close_conn();
                }
            }
            else if (events[i].events & EPOLLOUT) //可写入
            {
               
                if (!member[sockfd].write_file())
                {
                    member[sockfd].close_conn();
                }
            }
        }
    }
    close(epfd);
    close(listenfd);
    delete[] member;
    delete pool;
    return 0;
}