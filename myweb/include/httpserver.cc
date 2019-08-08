//
// Created by kiosk on 19-8-5.
//
#include "httpserver.h"
#include "boost/get_pointer.hpp"
#include "Tcpconnection.h"
#include "Buffer.h"
#include "HttpContext.h"
#include "httpresponse.h"
#include "threadpool.h"
httpserver::httpserver(Eventloop *loop) : loop_(loop) , acceptor_(new Acceptor(loop))
{
    en  = new threadpool(8);
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
    io->setsockfd(std::move(sockfd));
    Tcpptr conn (new Tcpconnection(loop_,sockfd));
    connections_.push_back(conn);
    loop_->runthisthread(std::bind(&Tcpconnection::connectget,conn));
}

