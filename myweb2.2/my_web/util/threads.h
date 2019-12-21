//
// Created by kiosk on 19-12-4.
//

#ifndef MYWEB_THREADS_H
#define MYWEB_THREADS_H
#include <vector>
#include <thread>
#include <utility>
class ConcurrencyProxy {
public:
    template <typename Callable, typename ...Args>
    explicit ConcurrencyProxy(Callable func, Args ...args) {
        thcont_.reserve(1);

        for (int i = 0; i < 1 ; ++i) {
            thcont_.emplace_back(std::forward<Callable>(func), std::forward<Args>(args)...);
        }
    }

    int wait() {
        for (auto &i: thcont_) {
            i.join();
        }

        return 0;
    }
private:
    std::vector<std::thread> thcont_;
};

#endif //MYWEB_THREADS_H
