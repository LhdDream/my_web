//
// Created by kiosk on 19-11-30.
//

#ifndef MYWEB_HTTP_MSG_HANDLER_H
#define MYWEB_HTTP_MSG_HANDLER_H
//对于http的接受和发送的操作
#include "http_response.h"
#include "http_request.h"
#include "../include/Buffer.h"

class Buffer;

class HTTPMessageParser;

class http_response;

class HttpMessageHandler {
public:
    explicit HttpMessageHandler() : conn_(std::make_unique<HTTPMessage>()), respon_(std::make_unique<http_response>()),
                                    parse_(std::make_unique<HTTPMessageParser>()) {
    }

    void RecvRequese(std::unique_ptr<Buffer> &Socket);

    void SendResponse(std::unique_ptr<Buffer> &Socket);

private:
    std::unique_ptr<HTTPMessage> conn_; //共同部分
    std::unique_ptr<http_response> respon_; // 回应
    std::unique_ptr<HTTPMessageParser> parse_; //解析
};

#endif //MYWEB_HTTP_MSG_HANDLER_H
