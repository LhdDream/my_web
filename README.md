
                                      　WebServer１.1
              简易 Web 服务器，实现了 GET、POST 等基本请求的解析与响应
---

概述
---

最近看 < < muduo > > 学习的练手项目，可以支持音频，图片等格式的get请求,采用了Reator +　线程池的结构，通过cgicc协议来编写cgi来与后台进行通讯
采用一个线程一个Eventloop循环，来处理时间，tcpserver来保存连接，缺少日志和定时器一些功能，后续会继续改进

---
  
特性
---
- 使用每一个线程一个事件循环，采用非阻塞的io,采用Reactor + 线程池的结构
- 利用状态机器去解析http请求，提高效率
- 性能并不好，需要继续修改完善

Webbench
---
![image](https://github.com/LhdDream/my_web/blob/master/myweb/picture/web_test.png)
   
