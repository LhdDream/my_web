//
// Created by kiosk on 19-7-30.
//
#ifndef MYWEB_BUFFER_H
#define MYWEB_BUFFER_H
//应用层必须使用 // http 请求

#include "net/Epoll_event.h"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <vector>


constexpr const size_t beginsize = 0;
constexpr const size_t filesize = 8192;

class Buffer {
public:
    explicit Buffer() : m_data(beginsize + filesize) {};

    size_t WriteAble() const { return m_data.capacity() - m_write_pos; }

    void ReSize(size_t len) {
        if (len > WriteAble()) {
            m_data.resize(len + m_data.size());
        }
    }

    void *BeginWrite() {
        return m_data.data() + m_write_pos;
    }

    void WriteOffest_Move(int len) {
        m_write_pos += len;
    }

    void *BeginRead() {
        return m_data.data() + m_read_pos;
    }
    void ReadOffest_Move(int len){
        m_read_pos += len;
    }
    std::vector<char> &Data() {
        return m_data;
    }

    void Reset() {
        m_data.clear();
        m_write_pos = 0;
        m_read_pos = 0;
    }

    int Write(void * src ,int len) {
        return Write(reinterpret_cast<const char * >(src) , len);
    }
    int Write(const char * src , int len){
        if (len > WriteAble()) {
            ReSize(len);
        }
        std::memcpy(m_data.data()+m_write_pos,src,len);
        WriteOffest_Move(len);
        return len;
    }
private:
    std::vector<char> m_data;
    size_t m_write_pos = 0; // 写指针
    size_t m_read_pos = 0 ; // 读指针
};

#endif //MYWEB_BUFFER_H
