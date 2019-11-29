//
// Created by kiosk on 19-7-30.
//

#ifndef MYWEB_CHANNEL_H
#define MYWEB_CHANNEL_H
#include "poll.h"
#include "../http/http_request.h"
#include "../http/http_response.h"
#include "Buffer.h"
#include "../util/Timer.h"
#include <memory>
#include <unordered_map>
#include <queue>
//每一个用户一个channel
//设置每一个用户的回调函数
//其中还有主线程也用到了channel 所以在这里设置一个回调

class channel_set;


class channel{
    friend  class channel_set;
public:
    using Callback = std::function<bool () > ;
    explicit channel (std::unique_ptr<Socket> &&sock_)
    :response_(std::make_unique<HTTPMessage>()),parse_(std::make_unique<HTTPMessageParser>()),Socket_(std::move(sock_)),
    ReadBuffer_(std::make_unique<Buffer>())
    {}
    explicit  channel() :response_(std::make_unique<HTTPMessage>()),parse_(std::make_unique<HTTPMessageParser>()),Socket_(std::move(sock_)),
                         ReadBuffer_(std::make_unique<Buffer>()),time_stamp_(0){}
    //如果声明析够函数,那么编译器不会主动声明移动构造函数
    int fd_() const ;
    void handleEvent_() ;
    void onRead_(Callback && re) ;
    void onWrite_(Callback && wr) ;

    //返回一个用户的时间戳
    void set_type_(EpollEventType && type) { type_ = std::move(type) };
    //设置一下epollevent的type选项
    EpollEventType  get_() { return type_;}
    //设置一下用户可以关闭的条件
    bool closeable(){
        return Read_ & Write_;
    }
private:
    std::unique_ptr<HTTPMessage> conn_ ; //共同部分
    std::unique_ptr<http_response> respon_ ; // 回应
    std::unique_ptr<HTTPMessageParser> parse_ ; //解析
    std::unique_ptr<Socket> Socket_ ; //对于每一个用户的fd进行保存
    std::unique_ptr<Buffer> ReadBuffer_; // 对于每一个用户的buffer
    //由read -> write 权限
    EpollEventType  type_ = EpollEventType ::KReadble; //epoll 的事件
    Callback Readcallback_  = nullptr;
    Callback Writecallback_ = nullptr;
    bool Read_ = false;
    bool Write_ = false;
};




//整个channel的集合
class channel_set{
public:
    explicit channel_set(poll && epoll) : epoll_(std::move(epoll)),
    Timer_(std::make_unique<TimerQueue>()){}
    int add(channel * & user);
    int add(channel * && user);
    void remove(channel *& user);
    void modify(channel *&user);
    void timelate();
    void doall(int id);
    void closechannel(int id);
private:
    poll epoll_;
    std::unordered_map<int,channel * > table_;
    //将所有的channel连接起来
    std::unique_ptr<TimerQueue> Timer_ ; // 定時器操作
    //定時器操作放在epoll之中，處理socket事件，同時也可以把定時器輪尋時間
    //一起執行
};

#endif //MYWEB_CHANNEL_H
