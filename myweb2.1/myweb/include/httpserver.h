//
// Created by kiosk on 19-8-5.
//

#ifndef MYWEB_HTTPSERVER_H
#define MYWEB_HTTPSERVER_H

#include "Acceptor.h"
#include "Eventloop.h"
#include <vector>
//在整个事件循环连接池中取出一个循环来进行使用
#include "newthread.h"
#include "Buffer.h"
class httpserver
{
public:
    httpserver(Eventloop * loop);
    void start() ;  //开启线程池
    ~httpserver() {printf("server 析够");}; //

     fixed_thread_pool * en;
private:
    std::unique_ptr<Eventloop> loop_; //事件循环 //主线程的循环
    void new_http(int sockfd);
    boost::scoped_ptr<Acceptor> acceptor_; // 独占式的
    Buffer buf;

    //监听客户端的连接请求
    //每一个线程都运行一个事件循环　one peer one loop
    //在这里进行生命周期的延长
};
#endif //MYWEB_HTTPSERVER_H
