//
// Created by kiosk on 19-8-2.
//
#include "httpresponse.h"
#include <iostream>
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

int HttpResponse::getfile(int fd)
{
    if(fd < 0)
    {
        return 0;
    }
    struct stat st;
    int cgi_output[2];
    int cgi_input [2];
    bzero(cgi_input,sizeof(int) *2);
    bzero(cgi_output,sizeof(int) *2);
    pid_t  tmp;
    Buffer output;
    if(path_.empty())
        path_ = "picture/index.html";
    if(path_[path_.size() - 1] == '/')
        path_ = path_ + "index.html"; //默认
     if(path_ != "picture/index.html")
         path_ =  "picture" + path_ ;
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
        if(send(fd,buffer,strlen(buffer),0) < 0)
        {
            std::cout << strerror(errno) <<std::endl;
        }
    }
    else {
        if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
        {
            cgi = 1;
        }
       // std::cout << "path_  " << path_  << "  cgi  " << cgi << std::endl;
        setstate(Ok);
        if(cgi == 0)
        {
            appendToBuffer(&output, fd);
            int fd_ = open(path_.c_str(), O_RDONLY);
            if (fd_ < 0) {
                perror("open error");
            }
            int number = 0;
            char buf[4028];
            while (number < st.st_size) {
<<<<<<< HEAD
                bzero(buf, sizeof(char) * 4000);
=======
                bzero(buf, sizeof(char) * 4028);
>>>>>>> f32684d77a537ce774fa89a7320ac557f50f1174
                number += read(fd_, buf, sizeof(char) * 4000);
                if (send(fd, buf, sizeof(char) * 4000, 0) < 0 && (strlen(buf) >0 ) && (fd > 0))
                    perror("send");
            }
            close(fd_);
         } else {
                if(pipe(cgi_output) < 0)
                {
                    perror("pipe cgi_out");
                }
                if(pipe(cgi_input) < 0)
                {
                    perror("pipe cgi_in");
                }
                if((tmp = fork()) < 0) //多线程本不该用fork
                {
                    perror("fork error");
                }
                if(tmp == 0)
                {
                    char meth_env[255];
                    char query_env[255];
                    char length_env[255];
                    bzero(meth_env,sizeof(char) *254);
                    bzero(query_env,sizeof(char) *254);
                    bzero(length_env,sizeof(char) *254);
                    //将子进程的输出由标准输出重定向到 cgi_ouput 的管道写端上
                    dup2(cgi_output[1], STDOUT_FILENO);
                    //将子进程的输出由标准输入重定向到 cgi_ouput 的管道读端上
                    dup2(cgi_input[0], STDIN_FILENO);
                    //关闭 cgi_ouput 管道的读端与cgi_input 管道的写端
                    close(cgi_output[0]);
                    close(cgi_input[1]);
                    if(method == 0)
                    {
                        strcpy(meth_env,"REQUEST_METHOD=GET");
                        sprintf(query_env, "QUERY_STRING=%s", query_.c_str());
                        putenv(query_env);
                    }
                    else
                    {
                        strcpy(meth_env,"REQUEST_METHOD=POST");
                        sprintf(length_env, "CONTENT_LENGTH=%d", body_.size());
                        putenv(length_env);
                    }
                    putenv(meth_env);
                    if(execl(path_.c_str(),path_.c_str(),NULL) <0)
                    {
                        perror("execle");
                    }
                    exit(0);
                }
                else
                {
                    close(cgi_output[1]);
                    close(cgi_input[0]);
                    if(write(cgi_input[1],body_.c_str(),body_.size()) <0 )
                    {
                        perror("write");
                    }

                    //然后从 cgi_output 管道中读子进程的输出，并发送到客户端去
                    std::string cgi_temp;
                    char c[1024];
                    bzero(c,sizeof(c));
                    int status = 1;
                    while (int ret =read(cgi_output[0], c, sizeof(c)) > 0)
                    {
                        cgi_number += ret;
                        cgi_temp += c;
                        bzero(c,sizeof(c));
                    }
                    appendToBuffer(&output, fd);
                    char cgi_tmp[65535];
                    bzero(cgi_tmp,sizeof(char) *65535);
                    strcpy(cgi_tmp,cgi_temp.c_str());
                    if(send(fd, cgi_tmp,sizeof(char) *65535 , 0) < 0)
                        perror("send  cgi ");
                    //关闭管道
                    close(cgi_output[0]);
                    close(cgi_input[1]);
                    //等待子进程的退出
                    waitpid(tmp, &status, 0);
                }
                cgi = 0;
        }

    }
    close(fd);
    return cgi;
}
void HttpResponse::appendToBuffer(Buffer* output,int fd)
{
    char buffer[64];
    struct stat st;
    if(stat(path_.c_str(),&st) < 0 )
    {
    }
    if(path_.find(cgi) != -1 )
    {
        st.st_size = cgi_number;

        std::cout << "cgi   " << cgi_number << std::endl;
    }
    bzero(buffer,sizeof(char)*64);
    snprintf(buffer, sizeof(char) * 64, "HTTP/1.1 %d OK", state_);
    output->append(buffer,strlen(buffer));
    output->append("\r\n",strlen("\r\n"));

//    // 消息长度
    bzero(buffer, sizeof(char) *64);
    snprintf(buffer, sizeof(char) * 64, "Content-Length: %d", st.st_size);
    output->append(buffer,strlen(buffer));
    output->append("\r\n",strlen("\r\n"));
    bzero(buffer, sizeof(char) *64);
    if(headers_["Connection"] != "Keep-Alive")
    {
        strcpy(buffer,"Connection: Close");
    }
    else
    {
        strcpy(buffer,"Connection: Keep-Alive");
    }
    output->append(buffer,strlen(buffer));
    output->append("\r\n",strlen("\r\n"));
    if(path_.find("png") != -1 )
        setContentType("image/png");
    else if(path_.find("img") != -1)
        setContentType("image/jpg");
    else if(path_.find("avi") != -1)
        setContentType("video/avi");
    else if(path_.find("mp4") != -1)
        setContentType("audio/mp4");
    else if(path_.find("pdf") !=  -1)
        setContentType("application/pdf");
    else if(path_.find("mp3") != -1)
    {
        setContentType("audio/mpeg");
    }
    for (const auto& header : headers_)
    {
        output->append(header.first.c_str(),header.first.size());
        output->append(": ",strlen("\r\n"));
        output->append(header.second.c_str(),header.second.size());
        output->append("\r\n",strlen("\r\n"));
    }
    output->append("\r\n",strlen("\r\n"));
    std::string c = output->get();
    char buf[512];
    bzero(buf,sizeof(char) *512);
    strcpy(buf,c.c_str());
    //std::cout <<"请求头："<<buf << std::endl ;
    if(send(fd,buf,strlen(buf),0) < 0 )
    {
        perror("send error");
    }
 //   std::cout <<"结束"<<std::endl ;
}
