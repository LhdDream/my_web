#include "channel.h"

//set ...............................
//if C++11
// user == left
// copy
// user  == rigth
// move

void User_set::Remove(int fd,EpollEventType type) {
    m_epoll.Remove_Channel({fd,type});
    close(fd);
}
void User_set::DoRead(int id) {
    auto &&user = getUser(id);
    auto it = user->m_Handler->RecvRequese( m_parse, m_respon);
    if (it == 2) {
        user->m_Type = Writeable();
        //这里设置为EPOLLIN | EPOLLOUT
        //处理之前的事件并且处理EPOLLIN事件
        m_epoll.Update_Channel({user->m_Socket,user->m_Type});
    }else if(it <= 0){
       Remove(id,user->m_Type);
        // man 7 epoll 中 read
        // if no fork
        //close == epoll _ remove
    }
}

void User_set::DoWrite(int id) {
    auto it = m_table[id]->m_Handler->SendResponse(m_respon);
    if(it == 0){
        //这里设置为EPOLLIN | EPOLLOUT
        //处理之前的事件并且处理EPOLLIN事件
        m_table[id]->m_Type = Readable();
        m_epoll.Update_Channel({id,Readable()});
    }
    if(it < 0){
        //如果长连接则改变状态,不然直接关闭
        Remove(id,m_table[id]->m_Type);
    }
}



