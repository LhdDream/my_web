//
// Created by kiosk on 19-8-2.
//
#include "Buffer.h"
#include "HttpContext.h"
#include <algorithm>

bool HttpContext::processRequestline(const char *begin, const char *end)
{
    bool sueccessful = false;
    const char * start = begin;
    const char * space =  std::find(start,end,' ');
    if(space != end && request_.setMethod(start,space))
    {
        start =  space + 1; //解析url
        space =  std::find(start,end,' ');
        if(space != end)
        {
            const char * que = std::find(start,space,'?');//跳过问号 // 问号式连接符号
           if(que == space) // 说明url  之中没有问号
           {
               request_.setPath(start,que);
           }
           else
           {
               request_.setPath(start,que);
               request_.setQuery(que,space);
           }
           que = std::find(start,end,'\r');
           sueccessful = std::equal(start,que-1,"HTTP/1.1");
        }
        if(sueccessful)
        {
            request_.setVersion(HttpRequest::KHTTP11);
        }
    }
    return sueccessful;
}

bool HttpContext::parseRequest(Buffer *buf)
{
    bool stat = true;
    bool fill = false;
    while(stat)
   {
       switch(state_)
       {
           case RequestLine:
           {
               const char * tmp = buf->find();
               if(tmp) // 不是空的
               {
                   fill = processRequestline(buf->peek(),tmp);
                   if(fill)
                   {
                       //说明第一行分析完毕
                       buf->retrievesetction(tmp + 2);
                       //说明buffer 缓冲区中的第一行读取完毕
                       state_ = Headers;
                   }
                   else
                   {
                       stat = false;
                   }
               }
               else
               {
                   stat = false; // 退出循环
               }
               break;
           }
           case Headers: {
               //一行一行读取
               const char * tmp = buf->find();
               if(tmp) // 不是空的
               {
                 // 执行读取头部的函数
                 // 以 : 来进行分割
                 const char * conn = std::find(buf->peek(),tmp,':');
                 if(conn)
                 {
                    request_.addHeaders(buf->peek(),conn,tmp);
                     buf->retrievesetction(tmp + 2); // 缓冲区往后偏移
                 }
                 else
                 {
                     stat = false;
                 }
               }
               else
               {
                   stat = false; // 退出循环
               }
               break;
           }
          /* case Body:
               break;*/
          // post 请求
       }
   }
    return !stat;
}