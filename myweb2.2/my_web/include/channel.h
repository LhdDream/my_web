//
// Created by kiosk on 19-7-30.
//

#ifndef MYWEB_CHANNEL_H
#define MYWEB_CHANNEL_H

#include "poll.h"
#include "Buffer.h"
#include "../http/http_msg_handler.h"
#include <unordered_map>
#include <queue>
#include <memory>
#include <utility>
#include "../util/sparsepp/spp.h"

using spp::sparse_hash_map;

class channel_set;

class poll;

//相当于user 和channel
class User {
    friend class channel_set;

public:
    using Callback = std::function<void()>;

    explicit User(int sock_)
            : Socket_(sock_),
              handler_(std::make_unique<HttpMessageHandler>(sock_)),
              type_(Readable()),
              Readcallback_(),
              Writecallback_() {}

    //如果声明析够函数,那么编译器不会主动声明移动构造函数

    void handleRead();

    void handleWrite();

    void onRead_(Callback &&re);

    void onWrite_(Callback &&wr);

private:
    int Socket_; //对于每一个用户的fd进行保存
    //我们可以把Socket和Buffer进行一个绑定
    //之后进行收发就可以直接
    std::shared_ptr<HttpMessageHandler> handler_; //对于http事件处理
    //由read -> write 权限
    EpollEventType type_; //epoll 的事件
    Callback Readcallback_;
    Callback Writecallback_;
};


//整个channel的集合
class channel_set {
public:
    explicit channel_set(std::shared_ptr<poll> epoll) : epoll_(std::move(epoll)),
                                                        respon_(std::make_unique<http_response>()),
                                                        parse_(std::make_unique<HTTPMessageParser>()) {}

    void add(int fd);

    void add(int fd, const std::shared_ptr<User> &c);

    void doRead(int id);

    void doWrite(int id);

    void loop(const std::vector<int> &namelist);

private:
    std::shared_ptr<poll> epoll_;
    sparse_hash_map<int, std::shared_ptr<User>> table_;
    std::unique_ptr<http_response> respon_; // 回应
    std::unique_ptr<HTTPMessageParser> parse_; //解析
    //唯一的
    //所有的fd和http_msg_handler 作为一个对象池
    //将所有的channel连接起来
    //定時器操作放在epoll之中，處理socket事件，同時也可以把定時器輪尋時間
    //一起執行
private:
    decltype(auto) Getoper(int fd) {
        if (table_.find(fd) == table_.end()) {
            //if http_msg_handler  to create it
            //只要让她在执行过程中找到相应的完整类别则可以
            //设置标志位,解析问题
            auto user = std::make_shared<User>(fd);
            table_.emplace(std::make_pair(fd, user));
            user->onRead_(
                    [this, user = user]() {
                        auto it = user->handler_->RecvRequese(parse_, respon_);
                        if (it == 2) {
                            user->type_ = Allof();
                            epoll_->add_channel({user->Socket_, user->type_});
                        }
                    }
            );
            user->onWrite_(
                    [this, user = user]() {
                        auto it = user->handler_->SendResponse(respon_);
                        if (it == 0) {
                            user->type_ = Readable();
                            epoll_->add_channel({user->Socket_, user->type_});
                        }
                    }
            );
        } else {
            table_[fd]->handler_->clear();
        }
        epoll_->add_channel({table_[fd]->Socket_, table_[fd]->type_});
    }
};


#endif //MYWEB_CHANNEL_H
