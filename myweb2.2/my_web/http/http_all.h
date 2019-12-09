//
// Created by kiosk on 2019/12/9.
//

#ifndef MYWEB_HTTP_ALL_H
#define MYWEB_HTTP_ALL_H
//使用享元模式把一部分内容进行抽象
//针对于每一个fd中对于http请求部分进行频繁的创建和销毁
#include <unordered_map>
#include "http_msg_handler.h"
//但是对象池必须使用单例模式
//针对与函数的抽象类的实现
class oper{
public:
    virtual  ~oper() = 0 ;
    virtual int RecvRequese(std::unique_ptr<HTTPMessageParser>& parse_,std::unique_ptr<http_response> &respon_) = 0;
    virtual int SendResponse(std::unique_ptr<http_response> &respon_) =  0;
    virtual  bool closeable() =  0;
};

class opership{
public:
private:
};
#endif //MYWEB_HTTP_ALL_H
