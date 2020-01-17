//
// Created by kiosk on 2019/12/9.
//
#include <queue>
#include "../include/Socket.h"
#ifndef MYWEB_TIMER_H
#define MYWEB_TIMER_H
//这里进行定时器的实现，对于超时连接进行淘汰
//采用最小堆的实现方式通过
class Timer final{
public:
    Timer() ;
    ~Timer() ;
    void AddTimer();
    void RemoveTimer();

private:

};
#endif //MYWEB_TIMER_H
