//
// Created by kiosk on 19-8-2.
//

#ifndef MYWEB_HTTPRESPONSE_H
#define MYWEB_HTTPRESPONSE_H
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "Buffer.h"
#include <string>
#include "map"
#include <sys/sendfile.h>
class HttpResponse
{
public:
    enum state
    {
        Unkown,
        Ok = 200,
        MovedPermanently = 301,
        BadRequest = 400,
        NotFound = 404,
    };
    HttpResponse(std::string path) : state_(Unkown),path_(path)
    {

    }
    ~HttpResponse() = default;
    void setstate(state code){state_ = code;}
    void setMessage(const std::string& message){Message_ = message;}

    void setContentType( )
    {
        addHeader("Content-Type", "Keep-alive");
    }

    void addHeader(const std::string& key, const std::string& value)
    {
        headers_[key] = value;
    }

    void appendToBuffer(Buffer* output,int fd) ;
    int getfile(int fd);

private:
    std::map<std::string,std::string> headers_;
    state  state_;
    std::string Message_;
    std::string path_;
    int cgi = 1;
};
#endif //MYWEB_HTTPRESPONSE_H
