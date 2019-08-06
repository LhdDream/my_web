//
// Created by kiosk on 19-8-6.
//

#ifndef MYWEB_THREADPOOL_H
#define MYWEB_THREADPOOL_H

#include <mutex>
#include <condition_variable>
#include <thread> //
#include <queue> // 任务队列
#include <functional> //
#include <atomic>
#include <vector>
#include <boost/enable_shared_from_this.hpp>
#include <typeinfo>
#include <future>
#include "Eventloop.h"
class threadpool : public boost::enable_shared_from_this<threadpool> //获取一个指向自身的shared_ptr 管理生命周期
{
public:
    explicit  threadpool(size_t thread_count ) : data_(std::make_shared<data>())
    {
        thread_count = ( thread_count < std::thread::hardware_concurrency() ) ? std::thread::hardware_concurrency() : thread_count;
        for(size_t i  = 0 ;i < thread_count ; i++)
        {
            thread_.emplace_back([data = data_]//值传递
                                 {
                                     std::unique_lock<std::mutex> beg(data->mutex_);
                                     while(true)
                                     {

                                         if(!data->task_.empty())
                                         {

                                             auto current = std::move(data->task_.front());

                                             data->task_.pop();

                                             beg.unlock();

                                             current();

                                             beg.lock();

                                             //如果一个线程执行完之后，不需要将其沉睡，可以让他继续执行
                                         }
                                         else if(data->stop_)
                                         {
                                             break;
                                         }
                                         else
                                         {
                                             data->cond_.wait(beg);
                                         }
                                     }
                                 });
        }
    }
    threadpool() = default;
    threadpool(threadpool &&) = default; // 默认的移动构造函数
    threadpool&operator=(const threadpool &) = default;
    //执行函数
    void sumblit();
    Eventloop * getNextLoop();
    void starts();
    ~threadpool();


private:
    struct data {
        std::atomic<bool> stop_;//等待线程是不是执行完毕
        std::mutex mutex_;
        std::queue<std::function<void()> > task_;
        std::condition_variable cond_;
    };
    std::vector<std::thread> thread_;
    //指向自身的指针
    std::shared_ptr<data> data_ ; //管理线程的对应周期
    int next_;
    std::vector <Eventloop * > loop_ ;
    Eventloop * loop ;
};
#endif //MYWEB_THREADPOOL_H
