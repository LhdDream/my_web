//
// Created by kiosk on 19-11-26.
//
#ifndef MYWEB_EPOLL_EVENT_H
#define MYWEB_EPOLL_EVENT_H
//对于epoll的事件的类
#include <sys/epoll.h>
#include <list>
#include <type_traits>
#include <vector>
#include <memory>

//对于epoll的事件处理的类
enum  EpollEventType  {
    KReadble = ::EPOLLIN,
    KWriteable = ::EPOLLOUT,
    KOneShot = ::EPOLLONESHOT,
    KET = ::EPOLLET,
    KClose = EPOLLRDHUP
}; // 使用enum class 来防止命名污染
//如果使用enum class 不会隐式int /float 类型，禁止
//底层使用long
//


//对于强类型进行位操作,需要重载 |
constexpr EpollEventType Basic() { return EpollEventType(EpollEventType::KET | EpollEventType::KReadble   ); }
constexpr  EpollEventType Oneshot() {return EpollEventType(EpollEventType::KOneShot);}

constexpr EpollEventType Readable() { return EpollEventType(Basic() | Oneshot()); }

constexpr EpollEventType Writeable() { return EpollEventType(EpollEventType::KWriteable | Basic()); }

constexpr EpollEventType Allof() { return EpollEventType(Readable() | Writeable()); }

//这个文件对于epoll的事件进行再次封装
class Epoll_event {
    friend class poll;

public:
    explicit Epoll_event(int fd) : event_(epoll_event{Allof(), {.fd = fd}}) {}

    Epoll_event() : event_{} {} //
    //
    Epoll_event(int fd, EpollEventType type) : event_(epoll_event{type, {.fd = fd}}) {}

    int event_fd() const { return event_.data.fd; }

    epoll_event *pointer() noexcept {
        return &event_;
    }

private:
    epoll_event event_;
};

//下面是对于整个epoll event 数组的一个简单封装
class EpollEventResult {
    friend class poll;

public:
    explicit EpollEventResult(int size) : store_(std::make_unique<std::vector<Epoll_event>>()) {
        store_->resize(size);
    }

    explicit EpollEventResult() : store_(std::make_unique<std::vector<Epoll_event>>()) {
        store_->resize(1024);
    }

    Epoll_event &operator[] (size_t i) {
        return store_->at(i);
    }

    size_t fillsize_() const {
        return store_->size();
    }
    void resize(size_t file) const{
        store_->resize(store_->size() + file);
    }
private:
    epoll_event *get() {
        return store_->begin()->pointer();
    }

    std::unique_ptr<std::vector<Epoll_event>> store_;
};


#endif //MYWEB_EPOLL_EVENT_H
