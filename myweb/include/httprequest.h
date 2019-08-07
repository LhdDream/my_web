//
// Created by kiosk on 19-8-1.
//

#ifndef MYWEB_HTTPREQUEST_H
#define MYWEB_HTTPREQUEST_H
#include <map>
#include <string>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "sys/stat.h"
#include <cstring>
class HttpRequest
{
public:
    enum Method
    {
        KInvalid,KGet,KPost
    };
    enum Version
    {
        KUnkown,KHTTP11
    };
    HttpRequest() :method_(KInvalid),version_(KUnkown)
    {

    }
    void setVersion(Version v)
    {
        version_ = v;
    }
    bool setMethod(const char* start, const char* end)
    {
        assert(method_ ==  KInvalid);
        std::string m(start, end);
        if (m == "GET")
        {
            method_ = KGet;
        }
        else if (m == "POST")
        {
            method_ = KPost;
        }
        return method_ != KInvalid;
    }
    //得到报文
    //解析请求报文
    void setPath(const char* start, const char* end)
    {
        path_.assign(start, end);
    }

    const std::string& path() const //  路径
    { return path_; }
    bool Methos() {return   method_ == KPost; }
    void setQuery(const char* start, const char* end)
    {
        query_.assign(start, end);
    }
    const std::string& query() const
    { return query_; }
    void addHeaders(const char * begin , const char * conn , const char * end)
    {
        if(begin == conn || conn == end)
        {
            return ;
        }
        std::string first(begin,conn);
        conn++;
        while(*conn == ' ' && conn < end)
        {
            conn++;
        }
        while(*end == ' ' && end > conn)
        {
            end--;
        }
        std::string value(conn,end);
        headers_[first] = value;
    }
    size_t  bodysize()
    {
        return std::stoi(headers_["Content-Length"]);
    }
    void  setbody(const char * begin,const char * end)
    {
        std::string body(begin,end);
        body_ = body;
    }
    std::string getbody()
    {
        return body_;
    }
private:
    std::string path_;
    std::string query_;
    Method  method_;
    Version version_;
    std::string body_;
    std::map<std::string,std::string> headers_;
};
#endif //MYWEB_HTTPREQUEST_H
