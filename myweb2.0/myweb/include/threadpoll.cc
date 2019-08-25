//
// Created by kiosk on 19-8-6.
//
#include "threadpool.h"

#include <mutex>
#include <condition_variable>
#include <vector>
#include "Eventloop.h"
    //执行函数


Eventloop * threadpool::getNextLoop()
{
    if (!loop_.empty())
    {
        Eventloop *loops = loop_[next_];
        ++next_;
        if (static_cast<size_t>(next_) >= loop_.size())
        {
            next_ = 0;
        }
        return loops;
    }
    return nullptr;
}

threadpool::~threadpool()
{
    if(data_ != NULL)
    {
        std::lock_guard<std::mutex> destory(data_->mutex_); //使用RAII管理
    }
    data_->cond_.notify_all();
    for(auto  & threads_ : thread_)  //这里应该使用左值传递
    {
        if(threads_.joinable())
            threads_.join();
    }
}

