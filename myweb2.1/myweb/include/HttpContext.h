//
// Created by kiosk on 19-8-2.
//

#ifndef MYWEB_HTTPCONTEXT_H
#define MYWEB_HTTPCONTEXT_H

#include "httprequest.h"
#include <atomic>
class Buffer;
class HttpContext
{
public:
    enum state // 头部
    {
        RequestLine,
        Headers,
        Body,
    };
    HttpContext() : state_(RequestLine),bodysize_(0)
    {
        cgi.store(0,std::memory_order_release);
    }
    ~HttpContext()
    {
    }
    bool parseRequest(Buffer* buf);
    const std::string & path()
    {
        return request_.path();
    };
    const std::string& query() const
    { return request_.query(); }
    int getcgi() { return cgi.load(std::memory_order_acquire);}
    std::string body() {return request_.getbody();}
private:
    bool processRequestline(const char * begin, const char * end);
//  解析请求
    state  state_;
    HttpRequest request_;
    std::atomic_int cgi;
    size_t  bodysize_;
};
#endif //MYWEB_HTTPCONTEXT_H
