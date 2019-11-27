//
// Created by kiosk on 19-8-5.
//
#include "httpserver.h"
#include "httpresponse.h"
#include "HttpContext.h"
#include "httprequest.h"
httpserver::httpserver(Eventloop *loop) : loop_(loop) , acceptor_(new Acceptor(loop_.get()))
{
    en  = new fixed_thread_pool(4);
    acceptor_->setCallback(std::bind(&httpserver::new_http,this,std::placeholders::_1));
}
void httpserver::start()
{
   if(!acceptor_->listening()) // 没有建立监听
   {
        loop_->runthisthread(std::bind(&Acceptor::listen,boost::get_pointer(acceptor_)));
   }
}

void httpserver::new_http(int sockfd)
{
    //这里应该使用epollout 事件，不然tcp 发送缓冲区满了之后没有办法处理
    //随后再改
    //开始第二个项目
    //在这里应该把解析的事件交给线程池来进行
    //发送的事件交给epoll来进行可写时间的执行
   
}

