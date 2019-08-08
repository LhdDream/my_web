//
// Created by kiosk on 19-7-30.
//

#ifndef MYWEB_EVENTLOOP_H
#define MYWEB_EVENTLOOP_H
#include <map>
#include <atomic>
#include <functional>
#include <vector>
#include <boost/any.hpp>
#include <thread>
#include "channel.h"
#include "poll.h"
#include <boost/scoped_ptr.hpp>
#include <unistd.h>
#include <sys/syscall.h>
#include "Socket.h"
#include <mutex>
#define gettid() syscall(__NR_gettid)
//在这里需要进行异步调用，如果不是本线程的需要压入栈中，之后在通过loop 进行执行
class channel;
class poll;
class Eventloop
{
public:
    typedef  std::function<void() > Funct;
    Eventloop();
    ~Eventloop();
    void loop() ; //事件循环
    void update(channel *) ;
    void runthisthread(Funct  cb)
    {
        if(threadid_ == gettid())
        {
            std::cout << "run this " << std::endl;
            cb();
        }
        else
        {
            {
                std::lock_guard<std::mutex> te(mutex_);
                funting.push_back(cb);
                //压入一个队列之中等进行自身的线程之后再次执行
            }
            if(!funting.empty() || !isthisthread()) {
                uint64_t one = 1;
                ssize_t n = ::write(channel_fd, &one, sizeof(one));
            }
        }
    };
    void dofucting();
    bool isthisthread() { return threadid_ == gettid();}
    void getSocket( int connfd);
    void setsockfd(int sockfd_) {sockfd = sockfd_ ; }
    int get() { return sockfd;}
private:
    int sockfd;
    std::atomic<bool> looping_;
    std::vector<channel *> channellist;
    std::unique_ptr<poll> poll_;
    std::unique_ptr<channel>  channel_;
    channel * chaneel_;
    int channel_fd ; //传递给channel 每一个时间循环中必须要有一个channel_
    //并且不能够提前释放，使用只能指针对其管理
    //在这里怕多线程模型中 poll 阻塞在　 poll _wait 之下
    pid_t  threadid_;
    int weak_fd;
    std::vector<Funct> funting;
    std::mutex mutex_;
};
#endif //MYWEB_EVENTLOOP_H
