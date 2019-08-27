//
// Created by kiosk on 19-8-5.
//
#include "httpserver.h"
#include "Tcpconnection.h"
#include "HttpContext.h"
#include "threadpool.h"
httpserver::httpserver(Eventloop *loop) : loop_(loop) , acceptor_(new Acceptor(loop_.get()))
{
    en  = new threadpool(4);
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
    Eventloop * io = en->getNextLoop();
    if(io == nullptr)
    {
        io = loop_.get();
    }
    Tcpptr conn (new Tcpconnection(io,sockfd));
    connections_.emplace_back(conn);
    io->runthisthread(std::bind(&Tcpconnection::connectget,conn));
}

