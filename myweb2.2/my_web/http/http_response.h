//
// Created by kiosk on 19-11-26.
//

#ifndef MYWEB_HTTP_RESPONSE_H
#define MYWEB_HTTP_RESPONSE_H
#include <cstdint>
#include "currency.h"
#include <fcntl.h>
#include <unordered_map>
#include <sys/stat.h>
#include <functional>
#include <sys/sendfile.h>
#include <unistd.h>
#include "../include/Buffer.h"
#include <climits>
#include "../include/Socket.h"

//static std::unordered_map< std::string_view,std::string> mime = {
//        {".html","text/html"},
//        {".avi","video/x-msvideo"},
//        {".bmp","image/bmp"},
//        {".png","image/png"},
//        {".txt","text/plain"},
//        {".mp3","audio/mp3"},
//        {".pdf","application/pdf"}
//};

constexpr  std::string_view  html = "text/html";
constexpr  std::string_view  avi = "video/x-msvideo";
constexpr  std::string_view  png = "image/png";
constexpr  std::string_view  txt = "text/plain";
constexpr  std::string_view  mp3 = "audio/mp3";
constexpr  std::string_view pdf  = "application/pdf";

inline std::string_view type(std::string_view p){
        if(p == html){
            return html;
        }else if(p == avi){
            return avi;
        }else if(p == png){
            return png;
        }else if(p == mp3){
            return mp3;
        }else if(p == pdf){
            return pdf;
        }else if(p == txt){
            return txt;
        }
        return html;
}

class http_response {
public:
    //组装response
    int  response(const std::unique_ptr <HTTPMessage > &httpMessage,const std::unique_ptr<Socket> &fd) {
        httpMessage->clear(false);
        struct stat64 st{};
        if (stat64(httpMessage->Getpath().data(), &st) < 0) {
            httpMessage->SetStatusCode(404);
        }
        httpMessage->SetStatusCode(200);
        std::string size = std::to_string(st.st_size);
        httpMessage->SetHeader("Content-Length", size);
        std::string_view temp =  httpMessage->Getpath();
        auto it = temp.find('.');
        httpMessage->SetHeader("Content-Type",type(temp.data()+ it +1));
        filesize = st.st_size;
        //接下来文件的读取和fast-cgi的处理
        //文件接口的处理
        //
         std::string_view  p ;
         httpMessage->ToString(&p);
        if(fd->write( p.data(),p.size(),0 ) < 0 )
        {
            if(errno != EAGAIN && errno != EINTR){
                return -1;
            }else{
                return -2;
            }
        }
        return 0;
    }
    //发送文件使用sendfile 循环
    void actsendfile(std::string_view path , int fd)
    {
        //发送文件的处理
        //必须先组成在发送文件
        int fd_ = open64(path.data(), O_RDONLY);
        off64_t offest = 0LL;
        while(offest < filesize){
            size_t  count;
            off64_t  remain = filesize - offest;
            if(remain > SSIZE_MAX)
            {
                count = SSIZE_MAX;
            }else{
                count = remain;
            }
            auto it = sendfile64(fd,fd_, &offest,count) ;
            if(it == - 1)
            {
                if(errno != EAGAIN && errno != EWOULDBLOCK)
                {
                    break;
                }
            }
        }
        close(fd_);
    }

private:
    size_t  filesize = 0;

};

#endif //MYWEB_HTTP_RESPONSE_H
