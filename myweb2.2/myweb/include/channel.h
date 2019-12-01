//
// Created by kiosk on 19-7-30.
//

#ifndef MYWEB_CHANNEL_H
#define MYWEB_CHANNEL_H

#include "poll.h"
#include "Buffer.h"
#include "../http/http_msg_handler.h"
#include <memory>
#include <unordered_map>
#include <queue>
//每一个用户一个channel
//设置每一个用户的回调函数
//其中还有主线程也用到了channel 所以在这里设置一个回调

class channel_set;


class channel {
    friend class channel_set;

public:
    using Callback = std::function<void()>;

    explicit channel(int sock_)
            : Socket_(std::make_unique<Socket>(sock_)),
              handler_(std::make_unique<HttpMessageHandler>()),
              ReadBuffer_(std::make_unique<Buffer>(sock_)) {}

    //如果声明析够函数,那么编译器不会主动声明移动构造函数
    int fd_() const;

    void handleRead();

    void handleWrite();

    void onRead_(Callback &&re);

    void onWrite_(Callback &&wr);

    //返回一个用户的时间戳
    void set_type_(EpollEventType &&type) { type_ = std::move(type); }

    //设置一下epollevent的type选项
    EpollEventType get_() { return type_; }

    //设置一下用户可以关闭的条件
    bool closeable() {
        return Read_ & Write_;
    }

private:
    std::unique_ptr<Socket> Socket_; //对于每一个用户的fd进行保存
    //我们可以把Socket和Buffer进行一个绑定
    //之后进行收发就可以直接
    std::unique_ptr<HttpMessageHandler> handler_; //对于http事件处理
    std::unique_ptr<Buffer> ReadBuffer_; // 对于每一个用户的buffer
    //由read -> write 权限
    EpollEventType type_ = Readable(); //epoll 的事件
    Callback Readcallback_ = nullptr;
    Callback Writecallback_ = nullptr;
    bool Read_ = false;
    bool Write_ = false;
};


//整个channel的集合
class channel_set {
public:
    explicit channel_set(poll &epoll) : epoll_(std::move(epoll)) {}

    int add(channel *&user);

    void remove(channel *&user);

    void doRead(int id);

    void doWrite(int id);

    void closechannel(int id);

private:
    poll epoll_;
    std::unordered_map<int, channel *> table_;
    //将所有的channel连接起来
    //定時器操作放在epoll之中，處理socket事件，同時也可以把定時器輪尋時間
    //一起執行
};

#endif //MYWEB_CHANNEL_H
