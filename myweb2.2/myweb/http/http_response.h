//
// Created by kiosk on 19-11-26.
//

#ifndef MYWEB_HTTP_RESPONSE_H
#define MYWEB_HTTP_RESPONSE_H
#include "currency.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <functional>
#include <sys/sendfile.h>
#include <unistd.h>
#include "Buffer.h"
using hash_t = uint64_t ;
constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;

constexpr size_t hash_compile_time(const char * str, hash_t last_value = basis)
{
    return *str ? hash_compile_time(str+1, (static_cast<hash_t >(*str) ^ last_value) * prime) : last_value;
}


class http_response
{
public:
    void response(HTTPMessage * httpMessage, int fd)
    {
        httpMessage->clear(false); // 除了路径之外都进行清除
        struct stat st{};
        if(stat(httpMessage->Getpath().c_str(),&st) < 0 )
        {
            httpMessage->SetStatusCode(404);
        }
        httpMessage->SetStatusCode(200);
        httpMessage->SetHeader("Content-Length",std::to_string(st.st_size));
        auto it = httpMessage->Getpath().find('.');
        std::string temp = httpMessage->Getpath().substr(it + 1,httpMessage->Getpath().size());
        Con_type(temp,httpMessage);
        //接下来文件的读取和fast-cgi的处理
        //文件接口的处理
        //

        std::string pos= httpMessage->ToString();
        if(send(fd,pos.c_str(),pos.size(),0))
        {
            std::cout << "send  .." << std::endl;
        }
        int fd_ = open(httpMessage->Getpath().c_str(),O_RDONLY);
        if(sendfile64(fd,fd_,nullptr,st.st_size) != st.st_size)
        {
            std::cout << "error" << std::endl;
        }
        close(fd_);
    }
    void  Con_type( const std::string& path,HTTPMessage * httpMessage)
    {
        switch (hash_compile_time(path.data()))
        {
            case hash_compile_time("png"):
             {
                 httpMessage->SetHeader("Content-Type","image/png");
                 break;
             }
            case hash_compile_time("img"):
            {
                httpMessage->SetHeader("Content-Type","image/jpg");
                break;
            }
            case hash_compile_time("avi"):
            {
                httpMessage->SetHeader("Content-Type","video/avi");
                break;
            }
            case hash_compile_time("mp4"):
            {
                httpMessage->SetHeader("Content-Type","audio/mp4");
                break;
            }
            case hash_compile_time("pdf"):
            {
                httpMessage->SetHeader("Content-Type","application/pdf");
                break;
            }
            case hash_compile_time("mp3"):
            {
                httpMessage->SetHeader("Content-Type","audio/mpeg");
                break;
            }
        }
    }

};
#endif //MYWEB_HTTP_RESPONSE_H
