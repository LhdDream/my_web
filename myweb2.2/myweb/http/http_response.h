//
// Created by kiosk on 19-11-26.
//

#ifndef MYWEB_HTTP_RESPONSE_H
#define MYWEB_HTTP_RESPONSE_H
#include <cstdint>
#include "currency.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <functional>
#include <sys/sendfile.h>
#include <unistd.h>
#include "../include/Buffer.h"
#include <climits>
#include "../include/Socket.h"

class Socket;
using hash_t = uint64_t;
constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;

constexpr size_t hash_compile_time(const char *str, hash_t last_value = basis) {
    return *str ? hash_compile_time(str + 1, (static_cast<hash_t >(*str) ^ last_value) * prime) : last_value;
}


class http_response {
public:
    //组装response
    int  response(HTTPMessage *httpMessage,std::shared_ptr<Socket> &fd) {
        httpMessage->clear(false); // 除了路径之外都进行清除
        struct stat64 st{};
        if (stat64(httpMessage->Getpath().c_str(), &st) < 0) {
            httpMessage->SetStatusCode(404);
        }
        httpMessage->SetStatusCode(200);
        httpMessage->SetHeader("Content-Length", std::to_string(st.st_size));
        auto it = httpMessage->Getpath().find('.');
        std::string temp = httpMessage->Getpath().substr(it + 1, httpMessage->Getpath().size());
        Con_type(temp, httpMessage);
        filesize = st.st_size;
        //接下来文件的读取和fast-cgi的处理
        //文件接口的处理
        //
        std::string p = httpMessage->ToString();
        if(fd->write( p.c_str(),p.size(),0 ) < 0 )
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
    void actsendfile(const std::string & path , int fd)
    {
        //发送文件的处理
        //必须先组成在发送文件
        int fd_ = open64(path.c_str(), O_RDONLY);
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
            if(sendfile64(fd,fd_, &offest,count) == 0 )
            {
                break;
            }
        }
        filesize = 0;
        close(fd_);
    }

private:
    void Con_type(const std::string &path, HTTPMessage *httpMessage) {
        switch (hash_compile_time(path.data())) {
            case hash_compile_time("png"): {
                httpMessage->SetHeader("Content-Type", "image/png");
                break;
            }
            case hash_compile_time("img"): {
                httpMessage->SetHeader("Content-Type", "image/jpg");
                break;
            }
            case hash_compile_time("avi"): {
                httpMessage->SetHeader("Content-Type", "video/avi");
                break;
            }
            case hash_compile_time("mp4"): {
                httpMessage->SetHeader("Content-Type", "audio/mp4");
                break;
            }
            case hash_compile_time("pdf"): {
                httpMessage->SetHeader("Content-Type", "application/pdf");
                break;
            }
            case hash_compile_time("mp3"): {
                httpMessage->SetHeader("Content-Type", "audio/mpeg");
                break;
            }
            default:
            {
                httpMessage->SetHeader("Content-Type","text/html");
                break;
            }
        }
    }

private:
    size_t  filesize = 0;
};

#endif //MYWEB_HTTP_RESPONSE_H
