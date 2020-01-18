#include "UserSet.h"

//set ...............................
//if C++11
// user == left
// copy
// user  == rigth
// move

void User_set::Remove(int fd) {
    m_epoll.Remove_Channel({fd,m_table[fd]->m_Type});
    close(fd);
}
void User_set::DoRead(int id, Timer & timer) {
    auto &&user = m_table[id];
    auto it = user->m_Handler->RecvRequese( m_parse, m_respon);
    if (it == 2) {//发送不完
        user->m_Type = Writeable();
        //处理之前的事件并且处理EPOLLIN事件
        m_epoll.Update_Channel({user->m_Socket,user->m_Type});
        timer.AddTimer(id);
    }
    else if(it == 1){
        timer.AddTimer(id);
        //更新定时器
        //长连接
    }
    else if(it <= 0){ // 短连接或者出错直接关闭
       Remove(id);
        // man 7 epoll 中 read
        // if no fork
        //close == epoll _ remove
    }
}

void User_set::DoWrite(int id, Timer & timer) {
    auto it = m_table[id]->m_Handler->SendResponse(m_respon);
    if(it == 2){
        //发送缓冲区还在
        timer.AddTimer(id);
    }
    else if(it == 1 ){
       //长连接修改状态
        m_table[id]->m_Type = Readable();
        m_epoll.Update_Channel({id,m_table[id]->m_Type });
        timer.AddTimer(id);
    }
    else if(it <= 0){
        Remove(id);
    }
}



