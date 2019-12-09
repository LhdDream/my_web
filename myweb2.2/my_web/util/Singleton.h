//
// Created by kiosk on 2019/12/9.
//

#ifndef MYWEB_SINGLETON_H
#define MYWEB_SINGLETON_H
//单例模式的实现
//使用std::once_flag
#include <memory>
#include <mutex>
template <typename T>
class Singleton{
public:
    Singleton() = default;
    Singleton(const Singleton &) = delete;
    Singleton&operator=(const Singleton&) = delete;
    ~Singleton() = default;

    static T&  Instance()
    {
        static std::once_flag flag;
        std::call_once(flag, [&](){ _ptr = std::make_unique<T>(); });
        return *_ptr;
    }
private:
    static std::unique_ptr<T> _ptr;
};
template <typename  T>
std::unique_ptr<T> Singleton<T>::_ptr = nullptr;
#endif //MYWEB_SINGLETON_H
