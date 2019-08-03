//
// Created by kiosk on 19-8-2.
//
#include "httpresponse.h"
#include <iostream>
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>

int HttpResponse::getfile(int fd)
{
    struct stat st;
    int cgi = 0;
    Buffer output;
    path_ = "picture" + path_;
    if(path_ == "picture")
        path_ = "picture/index.html";
    if(path_[path_.size() - 1] == '/')
        path_ = path_ + "index.html"; //默认
    //文件已经存在
    if(stat(path_.c_str(),&st) == -1) {
        setstate(NotFound);
        std::cout << strerror(errno) <<std::endl;
    }
    else {
//        if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
//            cgi = 1;
//        if (cgi == 0) {
            setstate(Ok);
            appendToBuffer(&output,fd);
            int fd_ = open(path_.c_str(), O_RDONLY);
            if(fd < 0 )
            {
                perror("open error");
            }
            int number = 0;
            char buf[1024];
            bzero(buf,sizeof(char) *1024);
            while(number < st.st_size)
            {
                number += read(fd_,buf,1024);
                if(send(fd,buf,strlen(buf),0) < 0 )
                    perror("send");
            }
            close(fd_);
    }
    return cgi;
}
void HttpResponse::appendToBuffer(Buffer* output,int fd)
{

    char buffer[32];
    struct stat st;
    if(stat(path_.c_str(),&st) < 0)
    {
        perror("stat error"  ) ;
    }
    bzero(buffer,sizeof(char)*32);
    snprintf(buffer, sizeof(char) * 32, "HTTP/1.1 %d ", state_);

    output->append(buffer,strlen(buffer));

    output->append("\r\n",strlen("\r\n"));

//    // 消息长度
    bzero(buffer, sizeof(char) *32);
    snprintf(buffer, sizeof(char) * 32, "Content-Length: %d", st.st_size);
    output->append(buffer,strlen(buffer));
    output->append("\r\n",strlen("\r\n"));

    setContentType();

    for (const auto& header : headers_)
    {
        output->append(header.first.c_str(),header.first.size());
        output->append(": ",strlen("\r\n"));
        output->append(header.second.c_str(),header.second.size());
        output->append("\r\n",strlen("\r\n"));
    }

    output->append("\r\n",strlen("\r\n"));
    std::string c = output->get();
    char buf[256];
    bzero(buf,sizeof(char) *256);
    strcpy(buf,c.c_str());
    if(send(fd,buf,strlen(buf),0) < 0 )
    {
        perror("send error");
    }

}
