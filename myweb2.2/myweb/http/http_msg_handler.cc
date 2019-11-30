//
// Created by kiosk on 19-11-30.
//
#include "http_msg_handler.h"

void HttpMessageHandler::RecvRequese(std::unique_ptr<Buffer> &Socket) {
    conn_ = std::make_unique<HTTPMessage>();
    Socket->readfd();
    parse_->Parse(conn_.get(),Socket->Get());
}

void HttpMessageHandler::SendResponse(std::unique_ptr<Buffer> &Socket) {
    respon_->response(conn_.get(),Socket->get_Fd());
    conn_.reset();
}