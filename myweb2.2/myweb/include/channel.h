//
// Created by kiosk on 19-7-30.
//

#ifndef MYWEB_CHANNEL_H
#define MYWEB_CHANNEL_H
#include "poll.h"
#include "../http/http_request.h"
#include "../http/http_response.h"
#include "Buffer.h"
#include <memory>
#include <unordered_map>
#include <queue>
//每一个用户一个channel
//设置每一个用户的回调函数
class channel{
public:
    using Callback = std::function<void () > ;
    explicit channel (std::unique_ptr<Socket> &&sock_,long stamp_)
    :response_(std::make_unique<HTTPMessage>()),parse_(std::make_unique<HTTPMessageParser>()),Socket_(std::move(sock_)),
    ReadBuffer_(std::make_unique<Buffer>()),time_stamp_(stamp_)
    {}
    explicit  channel() :response_(std::make_unique<HTTPMessage>()),parse_(std::make_unique<HTTPMessageParser>()),Socket_(std::move(sock_)),
                         ReadBuffer_(std::make_unique<Buffer>()),time_stamp_(0){}
    size_t fd() const ;

    void handleEvent() ;
private:
    std::unique_ptr<HTTPMessage> response_ ; //回应
    std::unique_ptr<HTTPMessageParser> parse_ ; //解析
    std::unique_ptr<Socket> Socket_ ; //对于每一个用户的fd进行保存
    std::unique_ptr<Buffer> ReadBuffer_; // 对于每一个用户的buffer
    long time_stamp_ ; // 每一个用户的事件戳
    EpollEventType  type;
};




//整个channel的集合
class channel_set{
public:
private:
    poll &epoll_;
    std::unordered_map<int,std::unique_ptr<channel>> table_;
};
#endif //MYWEB_CHANNEL_H
