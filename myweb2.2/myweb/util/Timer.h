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
    Timer(Callback &&cb ,time_t stamp,int fd) :callback_(std::move(cb)),
    expiration_(stamp), fd_(fd){
    }
    void run() const {
        callback_();
    }
    time_t  time_() const {
        return expiration_;
    }
private:
   Callback  callback_; // 每一个timerfd 的回调函数
   time_t expiration_; // 時間戳
   int fd_; // 用户id
};
class TimerQueue {
public:
    TimerQueue () = default;
    void AddTimer( Timer &&timer_)
    {
        Timerqueue_.emplace(std::move(timer_));
        //使用移动构造
    }
    void tick(){
        if(Timerqueue_.empty())
            return ;
        time_t curr = time(nullptr);
        // 公元1970到现在的秒数
        while(!Timerqueue_.empty() )
        {
            auto cc = Timerqueue_.top().time_();
            if( cc + 180 < curr)
            {
                    auto p = Timerqueue_.top();
                    p.run();
                    Timerqueue_.pop();
            }else{
                break;
            }
        }
    }
private:
    std::priority_queue<Timer> Timerqueue_;
    //优先队列只能在队尾添加，队首删除元素
};
#endif //MY_REDIES_TIMER_H
