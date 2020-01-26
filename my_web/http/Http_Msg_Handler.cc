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
        parse_.Parse(m_conn, m_Buffer.Data(), readsize);
    } else {
        return ReturnState::ERROR;
    }
    if (n == -1) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            return ReturnState::ERROR; // 从相应的点中删除
        }
    } else if (n == 0) {
        return ReturnState::ERROR;
    }
    return SendResponse(respon_);
}

int HttpMessageHandler::SendResponse(Http_Response &respon_) {
    auto c = respon_.Response(m_conn, m_sock);
    if (c == ReturnState::All_Connection || c == ReturnState::Fastcgi_Cgi) {
        if (c == ReturnState::All_Connection) {
            respon_.ActSendfile(m_conn->Getpath(), m_sock.Fd());
            //监听队列中删除掉内容
        }
        Clear();
        if (m_conn->Keep_Alive()) {
            m_conn->Set_Keep_Alive(false);
            return ReturnState::Long_Connection; // 说明为长连接
        }
    } else {
        if (c == ReturnState::ERROR) {
            return ReturnState::ERROR;//错误
        } else if (c == ReturnState::Buffer_Full) {
            return ReturnState::Buffer_Full;  // 注册epollout 事件
        }

    }
    return ReturnState::Short_Connection;//短连接
}