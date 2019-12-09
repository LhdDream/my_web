//
// Created by kiosk on 19-11-30.
//
#include "http_msg_handler.h"
int  HttpMessageHandler::RecvRequese(std::unique_ptr<HTTPMessageParser>& parse_,std::unique_ptr<http_response> &respon_) {

    read = false;
    int n ;
    int readsize = 0 ;
    for( n = 0 ; (n = sock_->read(Buffer_,4096,0)) > 0 ; ){
        readsize += n;
    }
    parse_->Parse(conn_.get(),Buffer_->data(),readsize);
    Buffer_->reset();
    read = true;
    if( n == -1)
    {
        if(errno != EAGAIN && errno != EWOULDBLOCK)
        {

            return -1; // 从相应的点中删除
        }
    }
    return SendResponse(respon_);
}

int  HttpMessageHandler::SendResponse( std::unique_ptr<http_response> &respon_) {
    write = false;
    auto c = respon_->response(conn_.get(),sock_);

    if(c  == 0) {
        respon_->actsendfile(conn_->Getpath(), sock_->fd());
        conn_->clear(true);
        write = true;

    }else{
        if(c == -1) {

            return -1;
        }else {
            if(!conn_->empty())
            {
                return 2;  // 注册epollout 事件
            }
        }
    }
    return 0;
}