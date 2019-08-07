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


class Buffer
{
 public:
        //初始大小和起始位置
        static   const size_t beginsize = 0;
        static   const size_t fillsize = 1024;
        explicit  Buffer():readindex(beginsize),writeindex(beginsize),data_(beginsize + fillsize)
        {};
        ~Buffer(){};
        const char *peek() const
        {  return begin() + readindex; }
        const char * begin() const
        {
            return &*data_.begin();
        }
        const char * end() const
        {
            return &*data_.end();
        }
        size_t writeableByte()
        {
            return 1024 - writeindex;
        }
        size_t readaleByte()
        {
            return writeindex-readindex;
        }
        size_t  preableByte()
        {
            return readindex;
        }
        const char * beginwrite()
        {
            return  begin()+writeindex;
        }
        void resize()
        {
            if(preableByte() > writeableByte())
            {
                int temp = writeindex-readindex;
                std::copy(data_.begin()+readindex,data_.begin()+ writeindex,data_.begin());
                //move data
                readindex = 0;
                writeindex = temp;
            }
            else
            {
                return ;
            }
        }
        void append(const char * tmp,size_t len)
        {
            resize();
            for(size_t i = 0 ; i < len ; i++)
            {
                data_[writeindex + i] = tmp[i];
            }
            writeindex += len ;
        };

        void clear() // 清空Buffer中的数据
        {
          data_.clear();
          data_.shrink_to_fit();
        }

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
        size_t writefd( char *buf, int n); // 相当于一次write操作
        size_t number(){return data_.size();}
        const char * find()
        {
            const char * test = std::search(peek(),beginwrite(),line,line+2);
            return test == beginwrite() ? NULL : test;
        }
        std::string get()
        {
            std::string d ;
            for(auto c :data_)
            {

               d += c;
            }
            return d;
        }

  private:
        std::vector<char> data_;//读取http请求
        size_t readindex;
        size_t writeindex;
        const char line[2] ={'\r','\n'};
};
#endif //MYWEB_BUFFER_H
