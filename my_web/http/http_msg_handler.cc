//
// Created by kiosk on 19-11-30.
//
#include "http_msg_handler.h"

 int
HttpMessageHandler::RecvRequese( HTTPMessageParser &parse_,  Http_Response &respon_) {

    int n;
    int readsize = 0;
    for (n = 0; (n = m_sock.Read(m_Buffer, 4096, 0)) > 0;) {
        readsize += n;
    }

    if(readsize > 0) {
        parse_.Parse(m_conn, m_Buffer->Data(), readsize);
    }else {
        return -1;
    }
    if (n == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            return -1; // 从相应的点中删除
        }
    }else if(n == 0){
        return -1;
    }
    return SendResponse(respon_);
}

int HttpMessageHandler::SendResponse( Http_Response &respon_) {
    auto c = respon_.Response(m_conn, m_sock);
    if (c == 0 ) {
       respon_.ActSendfile(m_conn->Getpath(), m_sock.Fd());
        //监听队列中删除掉内容
         Clear();
       //短连接没有必要删除
        //将msg中的内容进行删除
    } else {
        if (c == -1) {
            return -1;
        } else {
                return 2;  // 注册epollout 事件
        }
    }
    return 0;
}