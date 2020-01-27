//
// Created by kiosk on 2020/1/23.
//

#ifndef MY_WEB_FASTCGIHANDLER_H
#define MY_WEB_FASTCGIHANDLER_H

#include "Provider.h"
#include  "Fastcgi.h"
#include "net/Socket.h"
#include <map>

class HTTPMessage;
//与php-fpm进行连接，解析php ->> html 然后进行send发送
//每一个线程一个fastcgiHandler进行解析 或者每一个线程统一拥有一个
class FastCgiHandler{
public:
    explicit  FastCgiHandler() {


    }
    ~FastCgiHandler()  = default;



    void SendFastCgi(const std::unique_ptr<HTTPMessage> &conn);
    char * ReadFromPHP(int & size);
private:
    std::unique_ptr<FcgiHeader> MakeHeader(int type,int requestId,int contentLength,int paddingLength);
    std::unique_ptr<FcgiBeginRequestBody> MakeBeginRequestBody(int role,bool keepConnection);
    std::unique_ptr<unsigned  char []> MakeParam(int name_size , int value_size , const char * name ,const char * value);
    std::unique_ptr<unsigned  char []> MakeParam(std::string name,std::string value);
    bool SendEndRequestRecord();
    void SendBeginRequestBody();
    char * GetHtmlFromContent( char * content);
    static char * FindHtmlStart( char * content);



    Socket m_FastCgi_Socket;
    Buffer m_buffer;
};
#endif //MY_WEB_FASTCGIHANDLER_H
