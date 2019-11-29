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
enum class EpollEventType : uint32_t {
 KReadble = ::EPOLLIN,
 KWriteable = ::EPOLLOUT,
 KOneShot = ::EPOLLONESHOT,
 KET = ::EPOLLET
}; // 使用enum class 来防止命名污染
//如果使用enum class 不会隐式int /float 类型，禁止
//底层使用long
//
template <typename  E>
constexpr  typename  std::underlying_type<E>::type
toType(E enumerator) noexcept
{
    return static_cast<typename std::underlying_type<E>::type >(enumerator);
}

constexpr   EpollEventType operator|(EpollEventType f1 ,EpollEventType f2)
{
    return static_cast<EpollEventType >(toType(f1) | toType(f2));
}
//对于强类型进行位操作,需要重载 |
constexpr EpollEventType Basic() {return EpollEventType (EpollEventType::KET);}
constexpr EpollEventType Readable() { return EpollEventType (EpollEventType ::KReadble| Basic());}
constexpr EpollEventType Writeable() {return EpollEventType (EpollEventType::KWriteable | Basic());}
constexpr EpollEventType Allof() { return EpollEventType (Readable() | Writeable());}
//这个文件对于epoll的事件进行再次封装
class Epoll_event{
public:
    explicit Epoll_event(int fd) : event_(epoll_event{toType(Allof()),{.fd = fd}}){}
    Epoll_event() : event_{} {} //
    //
    Epoll_event(int fd,EpollEventType type) : event_(epoll_event{toType(type),{.fd = fd};}) {}

    int event_fd() const {return event_.data.fd;}
    bool ck(EpollEventType &ev) const { return (event_.events & toType(ev) ) != 0 ;}
    //检查事件
    bool ck(std::list<EpollEventType> &ev) const {
        for(const auto & c : ev)
        {
            if( (event_.events & toType(c) ) != 0)
            {
                return true;
            }
        }
        return false;
    }
    epoll_event * pointer() noexcept {
        return  &event_;
    }
private:
    epoll_event event_;
};

//下面是对于整个epoll event 数组的一个简单封装
class EpollEventResult {
public:
    explicit EpollEventResult(int size) : store_(std::make_unique<std::vector<Epoll_event>>(size)){}
    explicit  EpollEventResult() : store_(std::make_unique<std::vector<Epoll_event>>(1024)){}
    Epoll_event operator[] (int i) {return store_->at(i);}
    void resize() const {
        if(store_->size() >= store_->capacity() / 2)
        {
            store_->reserve(store_->size() *2);
        }
    }
    size_t  capacity() const   {
        return store_->capacity();
    }
private:
    epoll_event * get() {
        return reinterpret_cast<epoll_event *>(store_.get());
    }
    std::unique_ptr<std::vector<Epoll_event>> store_;
};
#endif //MYWEB_EPOLL_EVENT_H
