//
// Created by kiosk on 19-11-30.
//
#include "http_msg_handler.h"

int
HttpMessageHandler::RecvRequese(std::unique_ptr<HTTPMessageParser> &parse_, std::unique_ptr<http_response> &respon_) {
    std::cout << "recv fd " << sock_->fd() <<std::endl;
    int n;
    int readsize = 0;
    for (n = 0; (n = sock_->read(Buffer_, 4096, 0)) > 0;) {
        readsize += n;
    }
    if(readsize > 0) {
        parse_->Parse(conn_, Buffer_->data(), readsize);
    }
    if (n == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            close(sock_->fd());
            return -1; // 从相应的点中删除
        }
    }else if(n == 0){
        close(sock_->fd());
        return -1;
    }
    return SendResponse(respon_);
}
static int num = 0 ;
int HttpMessageHandler::SendResponse(std::unique_ptr<http_response> &respon_) {
    std::cout << "send" << num++ << std::endl;
    auto c = respon_->response(conn_, sock_);
    if (c == 0) {
        respon_->actsendfile(conn_->Getpath(), sock_->fd());
        close(sock_->fd()); // close文件的引用技术 -1 epoll会从
        //监听队列中删除掉内容
    } else {
        if (c == -1) {
            close(sock_->fd());
            return -1;
        } else {
                return 2;  // 注册epollout 事件
        }
    }
    return 0;
}