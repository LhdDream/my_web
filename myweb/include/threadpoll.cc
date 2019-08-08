//
// Created by kiosk on 19-8-6.
//
#include "threadpool.h"

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
    //执行函数


Eventloop * threadpool::getNextLoop()
{
    Eventloop* loops  =  data_->loop;

    if (!loop_.empty())
    {
        // round-robin
        loops = loop_[next_];
        ++next_;
        if (static_cast<size_t>(next_) >= loop_.size())
        {
            next_ = 0;
        }
    }
    return loops;
}

threadpool::~threadpool()
{
    if(data_ != NULL)
    {
        std::lock_guard<std::mutex> destory(data_->mutex_); //使用RAII管理
        data_->stop_ = true;
    }
    data_->cond_.notify_all();
    for(auto  & threads_ : thread_)  //这里应该使用左值传递
    {
        if(threads_.joinable())
            threads_.join();
    }
}

