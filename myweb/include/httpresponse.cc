//
// Created by kiosk on 19-8-2.
//
#include "httpresponse.h"
#include <iostream>
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
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
    //文件不存在
    if(stat(path_.c_str(),&st) == -1) {
        setstate(NotFound);
        appendToBuffer(&output,fd);
        char buffer[1024];
        bzero(buffer,sizeof(buffer));
        strcpy(buffer,"<html>\n"
                      "\n"
                      "<body >\n"
                      "\n"
                      "    <p>Not Found  </p><br>\n"
                      "    <p> The requested file was not found on this server</p><br>\n"
                      "</body>\n"
                      "\n"
                      "</html>");
        if(send(fd,buffer,strlen(buffer),0))
        {
            std::cout << strerror(errno) <<std::endl;
        }
    }
    else {
//        if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
//            cgi = 1;
//        if (cgi == 0) {
            std::cout << "path_  " << path_ << std::endl;
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
            std::cout << st.st_size<< std::endl;
            while(number < st.st_size)
            {
                bzero(buf,sizeof(char) * 1024);
                number += read(fd_,buf,sizeof(char )*1020);
                if(send(fd,buf,sizeof(char) * 1020 ,0) < 0 )
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
    if(path_.find("png") != path_.npos)
        setContentType("image/png");
    if(path_.find("img") != path_.npos)
        setContentType("image/jpg");
    if(path_.find("avi") != path_.npos)
        setContentType("video/avi");
    if(path_.find("mp4") != path_.npos)
        setContentType("audio/mp4");
    setContentType("Keep-alive");

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
