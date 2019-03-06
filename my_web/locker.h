//线程同步机制封装类
#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>
//封装信号量的类
class sem
{
    public:
        //创建并且初始化信号量
        sem()
        {
            if(sem_init(&m_sem,0,0) != 0)
            {
                //构造函数没有返回值，可以通过抛出异常来报告错误
                throw std::exception
            }
        }
}
/* code */
#endif //LOCKER_H
