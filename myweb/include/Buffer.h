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
        inline const char *peek() const
        {  return begin() + readindex; }
        const char * begin() const
        {
            return &*data_.begin();
        }
        size_t writeableByte()
        {
            return data_.size()-writeindex;
        }
        size_t readaleByte()
        {
            return writeindex-readindex;
        }
        size_t  preableByte()
        {
            return readindex;
        }
        std::string tostring()
        {
           std::string c ;
           for(size_t  i = 0; i < data_.size() ;i++)
           {
               c += data_[i];
           }
           return c;
        }
        const char * beginwrite()
        {
            return  begin()+writeindex;
        }
        void resize()
        {
            if(preableByte() > writeableByte())
            {
                std::copy(data_.begin()+readindex,data_.begin()+ writeindex,data_.begin());
                //move data
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
        };
        void apppend(std::string & temp)
        {
            resize();
            for(size_t i = 0 ; i < temp.size()-1 ; i++)
            {
                data_[writeindex + i] = temp[i];
            }
        };
        void append(const char tmp)
        {
            data_[writeindex] = tmp;
            writeindex++;
        }
        void clear() // 清空Buffer中的数据
        {
          data_.clear();
          data_.shrink_to_fit();
        }
        size_t  find()
        {
            size_t  tmp = 0;
            for(auto &c :data_)
            {
                if(c == interval[0] )
                {
                    return tmp;
                }
                tmp++;
            }

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
        void retrieveAll() // 把剩下的读取完毕
        {
            readindex= 0;
            writeindex = 0;
        }
        size_t readfd( char* vptr,size_t n); // 相当于一次read操作
        size_t writefd( char *buf, int n); // 相当于一次write操作
        size_t number(){return data_.size();}
        void getnospace(char * method)
        {
            int i = 0;
            for(auto c : data_)
            {
                if(c == ' ')
                {
                    break;
                }
                else
                {
                    c = method[i];
                    i++;
                }
            }
            std::cout << "method" << method << std::endl;
           // method[i] = '\0';
           // data_.erase(data_.begin(),data_.begin()+i);
        }
        void print()
        {
            for(auto c :data_)
            {
                std::cout << c ;
            }
            std::cout << "\n";
        }
  private:
        std::vector<char> data_;//读取http请求
        size_t readindex;
        size_t writeindex;
        static const char interval[] ;//间隔符号标志
};
#endif //MYWEB_BUFFER_H
