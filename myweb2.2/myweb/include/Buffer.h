//
// Created by kiosk on 19-7-30.
//
#ifndef MYWEB_BUFFER_H
#define MYWEB_BUFFER_H
//应用层必须使用 // http 请求
#include <vector>
#include <cstddef>
#include <string>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include "Epoll_event.h"

constexpr const size_t beginsize = 0;
constexpr const size_t fillsize = 4096;

//Buffer 进行输出和输入缓冲区的编写
class Buffer {
public:
    //初始大小和起始位置
    explicit Buffer(int fd) : readindex(beginsize), writeindex(beginsize), data_(beginsize + fillsize),
                              fd_(fd) {
        printf("Buffer %zu\n", data_.size());
    };

    ~Buffer() = default;

    char *begin() {
        return &*data_.begin();
    }

    const char *begin() const {
        return &*data_.begin();
    }

    size_t writeableByte() {
        printf("size  %zu %zu \n", data_.size(), writeindex);
        return data_.size() - writeindex;
    }

    size_t readaleByte() {
        return writeindex - readindex;
    }

    void resize(size_t len) {
        if (writeableByte() < len) {
            data_.resize((writeindex + len) * 2);
        } else if (readindex != 0) // 如果之前有空余那么将其移动
        {
            size_t readable = readaleByte();
            std::copy(begin() + readindex,
                      begin() + writeindex,
                      begin());
            readindex = 0;
            writeindex = readindex + readable;
        }
    }

    void append_(const char *tmp, size_t len) {
        resize(len);
        std::copy(tmp, tmp + len, data_.begin() + writeindex);
        writeindex += len;
    };

    ssize_t readfd(); // 相当于一次read操作
    void reset() noexcept {
        data_.resize(fillsize);
        readindex = beginsize;
        writeindex = fillsize;
    }

    std::vector<char> &Get() {
        return data_;
    }

    int get_Fd() const {
        return fd_;
    }

private:
    std::vector<char> data_;//读取http请求
    size_t readindex;
    size_t writeindex;
    int fd_; // 每一个用户的fd
};

#endif //MYWEB_BUFFER_H
