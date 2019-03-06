//半同步/半反应堆线程池的实现
//可以通过使用一个工作队列完全来解除主线程和工作线程的耦合关系
//主线程往工作队列中插入任务，工作线程必须通过竞争来取得任务并且执行它
//同步值的是程序完全按照代码序列的顺序来进行执行
//异步是程序的执行需要用系统事件来进行驱动
//异步处理I/O事件，同步处理客户端的逻辑事件
#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <list>
#include <exception>
#include <pthread.h>
#include <iostream>
//包含了线程同步的机制问题
#include "locker.hpp"
/* code */
using namespace std;

//C++在创建线程的时候，第三个参数必须使用静态函数
//在这里使用泛型编程，扩大代码复用
template < typename T >
class threadpool
{
  public:
    int thread_number;    //线程池中的数量
    int max_requests;     //请求队列中允许的最大请求数量
    pthread_t *threads;   //线程数组
    list<T *> work_queue; // 工作队列
    locker queuelocker;   //保护请求队列的互斥锁，需要确保每一个时刻只有一个线程在那里使用
    sem queuestat;        // 有没有任务需要处理,sem是lock.hpp中对于信号量的处理的类
    bool stop;            //是不是需要结束线程

  public:
    threadpool(int p_thread_number = 10, int p_max_requests = 500);
    ~threadpool();
    //添加任务
    bool add_thread(T *request);

  private:
    //工作线程运行的时候，需要一个函数，在队列中取出任务并且运行
    static void *thread_worker(void *arg);
    void run_thread();
};

template < typename T >
threadpool< T >::threadpool(int p_thread_number , int p_max_requests )
{
    //初始化数据
    thread_number = p_thread_number;
    stop = false;
    max_requests = p_max_requests;
    threads = NULL;

    if (p_thread_number <= 0 || p_max_requests <= 0)
    {
        throw exception();
    }
    //创建线程数组
    threads = new pthread_t[p_thread_number];
    if (!threads)
    {
        throw exception();
    }
    //创建p_thread_number个线程，并且都设置为
    //分离线程它没有被其他的线程所等待，自己运行结束了，线程也就终止了，马上释放系统资源。

    // 脱离线程:
    //     不向主线程返回信息, 不需要主线程等待.通过两种方法创建 : 调用pthread_detach;
    //     修改线程的属性.< -这里使用
    for (int i = 0; i < p_thread_number; i++)
    {
        cout << i << "th pthread\n"
             << endl;
        if (pthread_create(threads + i, NULL, thread_worker, this) != 0) //第四个表示传递的参数
        {
            delete[] threads;
            throw exception();
        }
        //在Web服务器中当主线程为每个新来的链接创建一个子线程进行处理的时候，主线程并不希望因为调用pthread_join而阻塞（因为还要继续处理之后到来的链接），这时可以在子线程中加入代码
        // pthread_detach(pthread_self())
        if (pthread_detach(threads[i]))
        {
            delete[] threads;
            throw exception();
        }
    }
}

template < typename T >
threadpool< T >::~threadpool()
{
    delete[] threads;
    stop = true;
}

template < typename T >
bool threadpool< T >::add_thread(T *request)
{
    //添加工作队列一定要加锁，因为它被所有线程共享，如果不限制加锁的话，会生成线程竞争问题
    queuelocker.lock();
    if(work_queue.size() > max_requests)
    {
        queuelocker.unlock();
        return false;
    }
    work_queue.push_back(request);
    queuelocker.unlock();
    queuestat.post();
    //用于线程同步
    //盒子中的信号量例子，二进制的信号量实际相当于互斥锁
    //信号量+1
    //在系统中，给予每一个进程一个信号量，代表每个进程当前的状态，
    //未得到控制权的进程会在特定地方被强迫停下来，等待可以继续进行的信号到来。
    //为了防止多个进程在访问共享资源为引发的问题。信号量可以协调进程对共享资源的访问，也就是用来保护临界资源的。
    //任一时刻只能有一个执行线程进入临界区。 
    return true;
}

template < typename T >
void * threadpool< T >::thread_worker(void * arg)
{
    threadpool *pool =(threadpool *) arg;
    pool->run_thread();
    return pool;
}

template < typename T >
void threadpool<T>::run_thread()
{
    while(stop == false)
    {
        queuestat.wait();//减少信号量
        queuelocker.lock();//互斥锁添加
        if(work_queue.empty())
        {
            queuelocker.unlock();//解锁
            continue;
        }
        T * request = work_queue.front();
        work_queue.pop_front();//取出一个任务
        queuelocker.unlock();//解锁
        if(request == NULL)
            continue;
       request->doit();//指行
    }
}
#endif //THREADPOOL_H
