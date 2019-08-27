//
// Created by kiosk on 19-7-30.
//
#ifndef MYWEB_BUFFER_H
#define MYWEB_BUFFER_H
//应用层必须使用 // http 请求
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
class Buffer
{
 public:
        //初始大小和起始位置
        static   const size_t beginsize = 0;
        static   const size_t fillsize = 1024;
        explicit  Buffer():readindex(beginsize),writeindex(beginsize),data_(beginsize + fillsize )
        {};
        ~Buffer(){};
        const char *peek() const
        {  return begin() + readindex; }
        char * begin()
        {
            return &*data_.begin();
        }
        const char * begin() const
        {
            return &*data_.begin();
        }
        size_t writeableByte()
        {
            return data_.size() - writeindex;
        }
        size_t readaleByte()
        {
            return writeindex-readindex;
        }
        const char * beginwrite()
        {
            return  begin()+writeindex;
        }
        void resize(size_t len)
        {
            if (writeableByte()  < len )
            {
                data_.resize((writeindex+len) *2);
            }
            else
            {
                size_t readable = readaleByte();
                std::copy(begin()+readindex,
                          begin()+writeindex,
                          begin());
                readindex = 0;
                writeindex = readindex + readable;
            }
        }
        void append_(const char * tmp,size_t len)
        {

            if(len > writeableByte())
            {
                resize(len);
            }
            std::copy(tmp,tmp+len,data_.begin()+writeindex);
            writeindex += len ;
        };
        void retrieve(size_t len)
        {
            if(readaleByte() < len)
            {
                return ;
            }
            if(readaleByte() > len)
            {
                readindex += len;
            }
            else if(readaleByte() == len)
            {
                retrieveAll();
            }
        }
        void retrievesetction(const char * tmp){
           size_t  len = tmp - peek();
           readindex += len;
        }
        void retrieveAll() // 把剩下的读取完毕
        {
            readindex= 0;
            writeindex = 0;
        }
        ssize_t readfd(int fd); // 相当于一次read操作
        const char * find()
        {
            const char * test = std::search(peek(),beginwrite(),line,line+2);
            return test == beginwrite() ? NULL : test;
        }
  private:
        std::vector<char> data_;//读取http请求
        size_t readindex;
        size_t writeindex;
        const char line[2] ={'\r','\n'};
};
#endif //MYWEB_BUFFER_H
