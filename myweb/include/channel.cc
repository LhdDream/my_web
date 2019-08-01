#include "channel.h"
#include "Buffer.h"
#include <functional>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <sys/sendfile.h>

#define SERVER_STRING "Server: kioskhttpd/1,0.0\r\n"

const int kReadEvent = EPOLLIN | EPOLLPRI;

void serve_file(int fd_,char *path);
size_t getline(int sock,Buffer buffer , int size);
void error(int client);
void execute_cgi(int fd_, char *path, char *method, char *query_string);

void channel::handleEvent() {
    if( events_ |= kReadEvent)
        channel::handleRead();
}


void channel::handleRead()
{

    int cgi = 0 ;// cgi 处理post 请求
    char method[64];
    char url[256];
    char * query_string = NULL;
    char path[256];
    struct stat st;
    Buffer readbuf_;
    bzero(method,sizeof(char) * 64);
    bzero(url,sizeof(char) * 256);
    bzero(path,sizeof(char) * 256);
    size_t numchars = getline(fd_,readbuf_,256);
    if(numchars <= 0)
    {
       // std::cout << "numchars  too small" << std::endl;
    }
    readbuf_.getnospace(method);
    //不是GET 也不是post
    std::cout << "method " << method     << std::endl;
    /*if(strcasecmp(method,"POST") == 0)
            cgi = 1;
    readbuf_.getnospace(url);
    if(strcasecmp(method,"GET") == 0 )
    {
        query_string = url;
        for(auto c : url)
        {
            if ( c == '?')
            {
                cgi = 1;
                *query_string ='\0';
                query_string++;
            }
            query_string++; // 分割出来 ? 之后的数据
        }
    }

    sprintf(path,"picture%s",url); //解析的路径
    //考虑目录和文件的情况

    if (path[strlen(path) - 1] == '/')
        strcat(path, "index.html");
    //获取文件的状态，保存到st中
    if (stat(path, &st) == -1) {//如果文件状态获取失败（文件不存在）
        //将http请求读取完毕之后抛弃
        while((numchars = getline(fd_,readbuf_,256)) != 2)
        {
            readbuf_.clear();
        }
    }
    else // 如果文件存在，考虑目录和非目录的情况
    {
        if((st.st_mode & S_IFMT) == S_IFDIR)
            strcat(path,"/index.html");
        if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
        {
            cgi = 1;
        }
        // 权限问题
        if(!cgi)
            serve_file(fd_,path);
        //else
            //execute_cgi(fd_, path, method, query_string);
    }
    close(fd_);*/
}

void serve_file(int fd_,char *path)
{
    Buffer readbuf_;
    size_t  numchars = 0;
    struct stat statbuf;
    stat(path,&statbuf);
    while((numchars = getline(fd_,readbuf_,256)) != 2)
    {
        readbuf_.clear();
    }
    // 解析静态文件，如果有问号则看作动态文件
    //填充头部和身体部位
    char buf[1024];
    bzero(buf,sizeof(char) * 1024);
    strcpy(buf,"HTTP/1.0 200 OK\r\n");
    strcat(buf,SERVER_STRING);
    strcat(buf,"Content-type: text/html\r\n");
    strcat(buf,"\r\n");
    int temp = send(fd_,buf,strlen(buf),0);
    if(temp < 0)
    {
        std::cout << strerror(errno) << std::endl;
    }
    int openfd_ = open(path,O_RDONLY);
    if(openfd_ <  0)
    {
        std::cout <<"error open " << std::endl;
        return ;
    }
    int ret = sendfile(fd_,openfd_,NULL,statbuf.st_size);
    if(ret  < 0)
    {
        std::cout << strerror(errno) << std::endl;
    }
}

size_t getline(int sock,Buffer buffer , int size)
{
    char c ;
    int n = 0;
    int i = 0;
    while((i < size - 1) && (c !='\n'))
    {
        n = recv(sock,&c,sizeof(char),0);
        if(n > 0)
        {
            if(c == '\r')
            {
                //如果下一个是\n
                n = recv(sock,&c,sizeof(char),MSG_PEEK);
                if((n > 0 ) && (c == '\n'))
                {
                    recv(sock,&c,sizeof(char),0);
                }
                else
                    c = '\n';
            }
            buffer.append(c);
            i++;
        }
        else
        {
            c = '\n';
            std::cout << strerror(errno ) << std::endl;
        }
    }
    return i;
}
