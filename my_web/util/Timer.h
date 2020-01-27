//
// Created by kiosk on 2019/12/9.
//
#include "../config/Provider.h"
#include "Priority_queue.h"
#include <chrono>
#include <functional>


#ifndef MYWEB_TIMER_H
#define MYWEB_TIMER_H

//这里进行定时器的实现，对于超时连接进行淘汰
class Timer final {
public:
    using Callback = std::function<void(int)>;
    using Interval = std::chrono::milliseconds;

    explicit Timer() = default;

    ~Timer() = default;

    void AddTimer(size_t fd) {
        m_queue.Push(fd, GetSteadyClockMs() + Provider::Get().User_Keep_Connection());
        //现在时间
    }

    void RemoveTimer() {
        //判断有没有超时
        //如果小于2048用户不启用
        if (m_queue.GetSize() <= Provider::Get().KeepConnectionNumber()) {
            return;
        } else {
            auto &&obj = m_queue.Top();
            uint64_t now_time = GetSteadyClockMs();
            if (obj.second < now_time) {
                m_Callback(obj.first);
                m_queue.Pop();
            }
        }
    }

    void SetCallback(Callback &&c) {
        m_Callback = std::move(c);
    }

private:
    uint64_t GetSteadyClockMs() {
        auto now_time = std::chrono::steady_clock::now();
        uint64_t now = (std::chrono::duration_cast<Interval>(now_time.time_since_epoch())).count();
        return now;
    }

    Callback m_Callback;
    Priority_queue m_queue;
};

#endif //MYWEB_TIMER_H
