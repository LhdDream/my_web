//
// Created by kiosk on 19-11-30.
//

#ifndef MYWEB_HTTP_MSG_HANDLER_H
#define MYWEB_HTTP_MSG_HANDLER_H
//对于http的接受和发送的操作
//同时也是每一个httpclient连接
#include "http_response.h"
#include "http_request.h"
#include "../include/Buffer.h"
#include "../include/Socket.h"
#include <queue>
class Socket;

class Buffer;

class HTTPMessageParser;

class http_response;

class HttpMessageHandler {
public:
    explicit HttpMessageHandler() : conn_(std::make_unique<HTTPMessage>()),
                                    Buffer_(std::make_shared<Buffer>())
                                    {
    }

    int RecvRequese(std::shared_ptr<Socket> &sock_,std::unique_ptr<HTTPMessageParser>& parse_,std::unique_ptr<http_response> &respon_);

    int SendResponse(std::shared_ptr<Socket> &sock_ , std::unique_ptr<http_response> &respon_);

        bool closeable() {
        return write & read;
       }
private:


    std::unique_ptr<HTTPMessage> conn_; //共同部分
    std::shared_ptr<Buffer> Buffer_; // 缓冲区 // ReadBuffer
    bool read = false;
    bool write = false;

};

#endif //MYWEB_HTTP_MSG_HANDLER_H
