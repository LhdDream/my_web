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

    int cgi = 0 ;// cgi ����post ����
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
    //����GET Ҳ����post
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
            query_string++; // �ָ���� ? ֮�������
        }
    }

    sprintf(path,"picture%s",url); //������·��
    //����Ŀ¼���ļ������

    if (path[strlen(path) - 1] == '/')
        strcat(path, "index.html");
    //��ȡ�ļ���״̬�����浽st��
    if (stat(path, &st) == -1) {//����ļ�״̬��ȡʧ�ܣ��ļ������ڣ�
        //��http�����ȡ���֮������
        while((numchars = getline(fd_,readbuf_,256)) != 2)
        {
            readbuf_.clear();
        }
    }
    else // ����ļ����ڣ�����Ŀ¼�ͷ�Ŀ¼�����
    {
        if((st.st_mode & S_IFMT) == S_IFDIR)
            strcat(path,"/index.html");
        if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
        {
            cgi = 1;
        }
        // Ȩ������
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
    // ������̬�ļ���������ʺ�������̬�ļ�
    //���ͷ�������岿λ
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
                //�����һ����\n
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
