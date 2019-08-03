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
    HttpContext() : state_(RequestLine)
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
        std::cout << request_.query() << std::endl;
        std::cout << request_.path() << std::endl;
    }
    const std::string & path()
    {
        return request_.path();
    };
private:
    bool processRequestline(const char * begin, const char * end);
//  解析请求
    state  state_;
    HttpRequest request_;
};
#endif //MYWEB_HTTPCONTEXT_H
