#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

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
        template  <class T,class ... Args>
        auto  sumblit(T && f,Args&& ... args) ->std::future<decltype(f(args...))>// f的右值引用
        {
            if(data_->stop_ == true)
                throw std::runtime_error("threadpool is stopped");
            using RetType = decltype(f(args...));
            auto task = std::make_shared<std::packaged_task<RetType()>>
            (std::bind(std::forward<T>(f),std::forward<Args>(args)...));
            std::future<RetType> future = task->get_future();
            {

                std::lock_guard<std::mutex> lock(data_->mutex_);
                //data_->task_.emplace(std::bind(std::forward<T>(f),std::forward<Args>(args)...));
                //如果不使用task 会在这里进行析构，执行get方法会失败
                data_->task_.emplace([task](){(*task)();});
            }
            data_->cond_.notify_one();
            return future;
        }
        ~threadpool()
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
};
#endif