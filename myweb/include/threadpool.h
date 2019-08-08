

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
        for(size_t i  = 0 ;i < thread_count ; i++)
        {
            thread_.emplace_back([&,data = data_]//值传递
                                 {
                                     std::unique_lock<std::mutex> beg(data->mutex_);
                                     while(true)
                                     {
                                         beg.unlock();
                                         {
                                             Eventloop loops;
                                             {
                                                 data->loop = &loops;
                                                 data->cond_.notify_one();
                                                 loop_.push_back(data->loop);
                                             }
                                             loops.loop();
                                         }
                                         beg.lock();
                                         if(data->stop_)
                                         {
                                             break;
                                         }
                                     }
                                 });
        }
    }
    threadpool() = default;
    threadpool(threadpool &&) = default; // 默认的移动构造函数
    threadpool&operator=(const threadpool &) = default;
    //执行函数
    Eventloop * getNextLoop();
    ~threadpool();
private:
    struct data {
        std::atomic<bool> stop_;//等待线程是不是执行完毕
        std::mutex mutex_;
        std::queue<std::function<void()> > task_;
        std::condition_variable cond_;
        Eventloop * loop ;

    };
    std::vector <Eventloop * > loop_ ;
    std::vector<std::thread> thread_;
    //指向自身的指针
    std::shared_ptr<data> data_ ; //管理线程的对应周期
    int next_;
};
#endif //MYWEB_THREADPOOL_H

