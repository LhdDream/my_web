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
    
    en->execute([&,sockfd](){
        HttpContext context;
        buf.readfd(sockfd);
        context.parseRequest(&buf);
        HttpResponse resp(context.path(), context.getcgi(), context.query(), context.body());
        resp.getfile(sockfd);
        buf.clear();
    });
}

