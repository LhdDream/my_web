//
// Created by kiosk on 19-11-26.
//
#ifndef MYWEB_EPOLL_EVENT_H
#define MYWEB_EPOLL_EVENT_H
//对于epoll的事件的类
#include <sys/epoll.h>

#include <array>
#include <memory>
#include <type_traits>

//对于epoll的事件处理的类
enum EpollEventType
{
    KReadable = ::EPOLLIN,
    KWriteable = ::EPOLLOUT,
    KET = ::EPOLLET,
    KClose = ::EPOLLRDHUP
};

//对于强类型进行位操作,需要重载 |
constexpr EpollEventType Basic() {
    return EpollEventType(EpollEventType::KET);
}

constexpr EpollEventType Listen() {
    return EpollEventType(EpollEventType::KET | EpollEventType::KReadable);
}

constexpr EpollEventType Readable() {
    return EpollEventType(Basic() | EpollEventType::KReadable);
}

constexpr EpollEventType Writeable() {
    return EpollEventType(EpollEventType::KWriteable | Basic());
}

//这个文件对于epoll的事件进行再次封装
class Epoll_event {
public:
    explicit Epoll_event(int fd) : m_event(epoll_event{Readable(), {.fd = fd}}) {
    }

    Epoll_event() : m_event{} {
    }  //
    //
    Epoll_event(int fd, EpollEventType type) : m_event(epoll_event{type, {.fd = fd}}) {
    }

    epoll_event* Pointer() noexcept {
        return &m_event;
    }

    int EventFd() const {
        return m_event.data.fd;
    }

private:
    epoll_event m_event;
};

//下面是对于整个epoll event 数组的一个简单封装
class EpollEventResult {
    friend class Epoll;

public:
    EpollEventResult() = default;

    Epoll_event& operator[](size_t i) {
        return m_store[i];
    }

private:
    epoll_event* get() {
        return m_store.data()->Pointer();
    }

    std::array<Epoll_event, 10240> m_store;
};

#endif  // MYWEB_EPOLL_EVENT_H
