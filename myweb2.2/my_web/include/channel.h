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
#include <unordered_map>

class channel_set;

class poll;

//相当于user 和channel
class User {
    friend class channel_set;

public:

    explicit User(int sock_)
            : Socket_(sock_),
              handler_(std::make_unique<HttpMessageHandler>(sock_)),
              type_(Readable()){}

    //如果声明析够函数,那么编译器不会主动声明移动构造函数
private:
    int Socket_; //对于每一个用户的fd进行保存
    //我们可以把Socket和Buffer进行一个绑定
    //之后进行收发就可以直接
    std::unique_ptr<HttpMessageHandler> handler_; //对于http事件处理
    //由read -> write 权限
    EpollEventType type_; //epoll 的事件
};


//整个channel的集合
class channel_set {
    using Callback = std::function<void(int fd)>;
    using CloseCallback = std::function<void(int fd,EpollEventType & type)>;
public:
    explicit channel_set(std::shared_ptr<poll> epoll) : epoll_(std::move(epoll)),
                                                        respon_(std::make_unique<http_response>()),
                                                        parse_(std::make_unique<HTTPMessageParser>()) {
        m_Readable_ = [this] (int fd) {
                auto user = getUser(fd);
                auto type_ = user->type_;
                auto it = user->handler_->RecvRequese( parse_, respon_);
                if (it == 2) {
                       user->type_ = Allof();
                     //这里设置为EPOLLIN | EPOLLOUT
                        //处理之前的事件并且处理EPOLLIN事件
                        epoll_->update_channel({user->Socket_,user->type_});
                }else if(it <= 0){
                    m_Closeable_(fd,type_);
                    // man 7 epoll 中 read
                    // if no fork
                    //close == epoll _ remove
                }
        };
        m_Writeable_ = [this](int fd){
            auto it = table_[fd]->handler_->SendResponse(respon_);
            if(it <= 0){
                //如果长连接则改变状态,不然直接关闭
                m_Closeable_(fd,table_[fd]->type_);
            }
        };
        m_Closeable_ = [this](int fd,EpollEventType & type) {
            remove(fd,type);
            close(fd);
        };
    }

    void add(int fd);

    void remove(int fd,EpollEventType type);

    void doRead(int id);

    void doWrite(int id);

     std::shared_ptr<User>  getUser(int id) {
        if(table_.find(id) == table_.end()){
            auto &&user = std::make_shared<User>(id);
            table_.emplace(id,user);
        }
        return table_[id];
    }
private:
    std::shared_ptr<poll> epoll_;
    std::unordered_map<int, std::shared_ptr<User>> table_;
    std::unique_ptr<http_response> respon_; // 回应
    std::unique_ptr<HTTPMessageParser> parse_; //解析
    //唯一的
    //所有的fd和http_msg_handler 作为一个对象池
    //将所有的channel连接起来
    //定時器操作放在epoll之中，處理socket事件，同時也可以把定時器輪尋時間
    //一起執行
    Callback  m_Readable_;
    Callback  m_Writeable_;
    CloseCallback m_Closeable_;
};


#endif //MYWEB_CHANNEL_H
