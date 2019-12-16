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

constexpr  const size_t  beginsize = 0;
constexpr  const size_t  filesize = 1024;

class Buffer {
public:
    explicit  Buffer():data_(beginsize + filesize )
    {};

    size_t writeable() const {return data_.size() - write_pos_;}
    void resize(size_t len)
    {
       if(len > writeable())
       {
           data_.resize(len + data_.size());
       }else if(read_pos_ >  0){
           std::copy(data_.begin() + read_pos_,data_.begin() + write_pos_,data_.begin());
           read_pos_ = 0;
           write_pos_ = data_.size();
       }
    }



    void  * beginwrite() {
        return &*data_.begin() +write_pos_;
    }

    void writeoffest_move(int len){
        write_pos_ += len;
    }

    std::vector<unsigned char> &data() {
        return data_;
    }
    void reset() {
        data_.resize(data_.size());
        write_pos_ = 0;
        read_pos_ = 0;
    }
private:
    std::vector<unsigned char> data_;
    size_t write_pos_ = 0; // 写指针
    size_t read_pos_ = 0; // 读指针
};

#endif //MYWEB_BUFFER_H
