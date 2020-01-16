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


class poll;

//相当于user 和channel
class User {
    friend class User_set;

public:

    explicit User(int sock_)
            : m_Socket(sock_),
              m_Handler(std::make_unique<HttpMessageHandler>(sock_)),
              m_Type(Readable()){}

    //如果声明析够函数,那么编译器不会主动声明移动构造函数
private:
    int m_Socket; //对于每一个用户的fd进行保存
    //我们可以把Socket和Buffer进行一个绑定
    //之后进行收发就可以直接
    std::unique_ptr<HttpMessageHandler> m_Handler; //对于http事件处理
    //由read -> write 权限
    EpollEventType m_Type; //epoll 的事件
};


//整个channel的集合
class User_set {

public:
    explicit User_set( poll& epoll) : m_epoll(epoll),
                                                        m_respon{},
                                                        m_parse{} {
    }

     void Remove(int fd,EpollEventType type);

    void DoRead(int id);

    void DoWrite(int id);
    std::unique_ptr<User> & getUser(int id) {
        if(m_table.find(id) == m_table.end()){
            m_table.emplace(std::make_pair(id,std::make_unique<User>(id)));
        }
        return m_table[id];
    }
private:
    poll& m_epoll;
    Http_Response m_respon; // 回应
    HTTPMessageParser m_parse; //解析
    //唯一的
    //所有的fd和http_msg_handler 作为一个对象池
    //将所有的channel连接起来
    //定時器操作放在epoll之中，處理socket事件，同時也可以把定時器輪尋時間
    //一起執行
    std::unordered_map<int, std::unique_ptr<User>> m_table;
};


#endif //MYWEB_CHANNEL_H
