//
// Created by kiosk on 19-8-3.
//
#include "Acceptor.h"

void Acceptor::Listen() {
    m_acceptSocket.Listen();
}

void  Acceptor::HandleRead() //套接字可读的状态
{
    //ET使用accept来进行
    while (true) {
        int connfd = m_acceptSocket.Accpet();
        if (connfd > 0) {
            p(connfd);
        }
        if (connfd < 0) {
            //这里处理一下，服务器端如果文件描述符耗尽来进行的操作
            //来打开这里来进行处理
            if (errno == EMFILE) {
                ::close(m_idlefd);
                m_idlefd = m_acceptSocket.Accpet();
                ::close(m_idlefd);
                m_idlefd = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
            }  else
                break;
        }
    }
}
