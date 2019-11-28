//
// Created by kiosk on 19-10-4.
//

#ifndef MY_Web_TIMER_H
#define MY_Web_TIMER_H
//Where I plan to use timerfd and epoll to contact
#include <functional>
#include <memory>
#include <sys/timerfd.h>
#include <utility>
#include <queue>
//使用等待队列编写定时器
class Timer
{
public:
    using Callback = std::function<void()> ;
    explicit  Timer(Callback &&cb ,uint64_t stamp,int fd) :callback_(std::move(cb)),
    expiration_(stamp),
    clientfd_(fd){
    }
    void run() const {
        callback_();
    }
    void restart(uint64_t now)  {
        if( interval_ > 0 )
        {
            expiration_ = now + interval_;
        }
        else
        {
            expiration_ = 0 ;
        }
    }
    uint64_t  time_()
    {
        return expiration_;
    }
private:
   Callback  callback_; // 每一个timerfd 的回调函数
   uint64_t expiration_; //
   uint64_t interval_  = 3600;   //间隔时间
   int clientfd_ ; //客户端的fd
};
class TimerQueue {
public:
    using TimerPtr = std::shared_ptr<Timer>;
    explicit TimerQueue() = default;
    void AddTimer( TimerPtr &&timer_)
    {
        if(!timer_)
            return ; //防止nullptr指针
        Timerqueue_.emplace(std::move(timer_));
        //使用移动构造
    }
    void DelTimer()
    {
        if(!Timerqueue_.empty())
            Timerqueue_.pop();
        //删除优先队列中的第一个元素
    }
    void tick(){
        if(Timerqueue_.empty())
            return ;
        time_t curr = time(nullptr);
        // 公元1970到现在的秒数
        while(!Timerqueue_.empty())
        {
            auto tt = Timerqueue_.top();
            if(curr < tt->time_())
            {
                break; //  都没有过期
            }
            else
            {
                tt->run();
                Timerqueue_.pop();
            }
        }
    }
private:
    std::priority_queue<TimerPtr> Timerqueue_;
    //优先队列只能在队尾添加，队首删除元素
};
#endif //MY_REDIES_TIMER_H
