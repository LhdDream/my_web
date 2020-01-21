//
// Created by kiosk on 19-11-30.
//
#include "Http_Msg_Handler.h"

int
HttpMessageHandler::RecvRequese(HTTPMessageParser &parse_, Http_Response &respon_) {

    int n;
    int readsize = 0;
    for (n = 0; (n = m_sock.Read(m_Buffer, 1024, 0)) > 0;) {
        readsize += n;
    }

    if (readsize > 0) {
        parse_.Parse(m_conn, m_Buffer->Data(), readsize);
    } else {
        return -1;
    }
    if (n == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            return -1; // 从相应的点中删除
        }
    } else if (n == 0) {
        return -1;
    }
    return SendResponse(respon_);
}

int HttpMessageHandler::SendResponse(Http_Response &respon_) {
    auto c = respon_.Response(m_conn, m_sock);
    if (c == 0) {
        respon_.ActSendfile(m_conn->Getpath(), m_sock.Fd());
        //监听队列中删除掉内容
        Clear();
        if (m_conn->Keep_Alive()) {
            m_conn->Set_Keep_Alive(false);
            return 1; // 说明为长连接
        }
    } else {
        if (c == -1) {
            return -1;//错误
        } else {
            return 2;  // 注册epollout 事件
        }
    }
    return 0;//短连接
}