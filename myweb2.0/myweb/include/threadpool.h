#ifndef  MYWEB_THREADPOOL_H
#define  MYWEB_THREADPOOL_H
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
    explicit  threadpool(size_t thread_count ) : data_(std::make_shared<data>()),next_(0)
    {
        for(size_t i  = 0 ;i < thread_count ; i++)
        {
            thread_.emplace_back([&,data = data_]//值传递
                                  {
                                     Eventloop loops;
                                     {
                                         std::lock_guard<std::mutex> beg(data->mutex_);
                                         data->loop = &loops;
                                         loop_.push_back(data->loop);
                                     }
                                     loops.loop();
                                  }
                                 );
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
        std::mutex mutex_;
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

