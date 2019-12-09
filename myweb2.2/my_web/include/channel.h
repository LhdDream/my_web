//
// Created by kiosk on 19-7-30.
//

#ifndef MYWEB_CHANNEL_H
#define MYWEB_CHANNEL_H

#include "poll.h"
#include "Buffer.h"
#include "../http/http_msg_handler.h"
#include <memory>
#include "../util/sparsepp/spp.h"
#include <queue>
using spp::sparse_hash_map;
//每一个用户一个channel
//设置每一个用户的回调函数
//其中还有主线程也用到了channel 所以在这里设置一个回调

class channel_set;


class channel {
    friend class channel_set;

public:
    using Callback = std::function<void  ()>;

    explicit channel(int sock_)
            : Socket_(sock_),
              handler_(std::make_unique<HttpMessageHandler>()),
              type_(Readable()){}

    //如果声明析够函数,那么编译器不会主动声明移动构造函数
    int fd_() const;

   void handleRead();

    void  handleWrite();

    void onRead_(Callback &&re);

    void onWrite_(Callback &&wr);


    bool closeable() {
        std::cout << handler_.get() << std::endl;
        return handler_->closeable();
    }

    Epoll_event  get_event(){
        return Epoll_event{Socket_,type_};
    };


private:
    int Socket_; //对于每一个用户的fd进行保存
    //我们可以把Socket和Buffer进行一个绑定
    //之后进行收发就可以直接
    std::unique_ptr<HttpMessageHandler> handler_; //对于http事件处理
    //由read -> write 权限
    EpollEventType type_; //epoll 的事件
    Callback Readcallback_ = nullptr;
    Callback Writecallback_ = nullptr;
};


//整个channel的集合
class channel_set {
public:
    explicit channel_set(const std::shared_ptr<poll> &epoll) : epoll_(epoll),respon_(std::make_unique<http_response>()),
                                        parse_(std::make_unique<HTTPMessageParser>()){}

    void  add(const std::shared_ptr < channel >& user);

    void remove(channel *&user);

    void doRead(int id);

    void doWrite(int id);

    void closechannel(int id);

    void remove(int id);

    void run(EpollEventResult &result, size_t *user_number);
private:
    std::shared_ptr<poll> epoll_;
    sparse_hash_map<int, channel *> table_;
    std::unique_ptr<http_response> respon_; // 回应
    std::unique_ptr<HTTPMessageParser> parse_; //解析
    //将所有的channel连接起来
    //定時器操作放在epoll之中，處理socket事件，同時也可以把定時器輪尋時間
    //一起執行
};

#endif //MYWEB_CHANNEL_H
