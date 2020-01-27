//
// Created by kiosk on 19-11-30.
//

#ifndef MYWEB_HTTP_MSG_HANDLER_H
#define MYWEB_HTTP_MSG_HANDLER_H
//对于http的接受和发送的操作
//同时也是每一个httpclient连接
//在这里也是对于每一个用户的处理
#include "core/Buffer.h"
#include "Http_Response.h"
#include "Http_Request.h"
#include "net/Socket.h"
#include <thread>


class Buffer;

class HTTPMessageParser;


class HttpMessageHandler {
public:
    explicit HttpMessageHandler(int fd) :
            m_sock(fd),
            m_conn(std::make_unique<HTTPMessage>()),
            m_Buffer() {
    }

    int RecvRequese(HTTPMessageParser &parse_, Http_Response &respon_);

    int SendResponse(Http_Response &respon_);

    void Clear() {
        m_Buffer.Reset();
        m_conn->Clear();
    }

private:
    Socket m_sock; // 每一个用户的指向
    std::unique_ptr<HTTPMessage> m_conn; //共同部分
    Buffer m_Buffer; // 缓冲区 // ReadBuffer
};


#endif //MYWEB_HTTP_MSG_HANDLER_H
