//
// Created by kiosk on 19-8-2.
//

#ifndef MYWEB_HTTPCONTEXT_H
#define MYWEB_HTTPCONTEXT_H

#include "httprequest.h"
class Buffer;
class HttpContext
{
public:
    enum state // 头部
    {
        RequestLine,
        Headers,
        Body,
        GotAll
    };
    HttpContext() : state_(RequestLine),bodysize_(0)
    {
    }
    ~HttpContext()
    {
    }
    bool gotall()
    {
        return state_ == GotAll;
    }
    HttpRequest& request()
    {
        return request_;
    }
    bool parseRequest(Buffer* buf);
    void print()
    {
        std::cout <<  "body " << request_.getbody() << std::endl;
        std::cout << "bodysize " << request_.bodysize() << std::endl;
    }
    const std::string & path()
    {
        return request_.path();
    };
    const std::string& query() const
    { return request_.query(); }
    int getcgi() { return cgi;}
    std::string body() {return request_.getbody();}
private:
    bool processRequestline(const char * begin, const char * end);
//  解析请求
    state  state_;
    HttpRequest request_;
    int cgi = 0;
    size_t  bodysize_;
};
#endif //MYWEB_HTTPCONTEXT_H
