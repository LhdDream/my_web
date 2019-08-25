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
    HttpResponse(std::string path,int cgi_,std::string query,std::string body) : state_(Unkown),cgi(cgi_),method(cgi_),path_(path),query_(query),body_(body)
    {
    }
    ~HttpResponse()
    {};
    void setstate(state code){state_ = code;}
    void setContentType( const std::string & value)
    {
        addHeader("Content-Type",value);
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
    int method;
    int cgi = 0;
    int cgi_number = 0;
    std::string path_;
    std::string query_;
    std::string body_;
};
#endif //MYWEB_HTTPRESPONSE_H
