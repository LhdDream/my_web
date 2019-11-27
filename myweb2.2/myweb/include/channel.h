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

//每一个用户一个channel
class channel{
public:
private:
};
//整个channel的集合
class channel_set{
public:
private:
    poll &epoll_;
    std::unordered_map<int,std::unique_ptr<channel>> table_;
};
#endif //MYWEB_CHANNEL_H
