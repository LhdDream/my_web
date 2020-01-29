# A C++ High Performance Web Server

本项目是运行在Linux平台上的Web服务器,它由C++17编写,目前支持处理HTTP/1.1的静态GET请求以及FastCGI请求,支持HTTP长连接.

| Part Ⅰ   | Part Ⅱ     | Part Ⅲ   | Part Ⅳ   | Part V   |
| -------- | ---------- | -------- | -------- | -------- |
| 并发模型 | 改进和测试 | 项目历史 | 项目目的 | 自我反思 |

# 特性:


- `Reactor`风格
- 基于`事件驱动`模型
- C++设计模式中的`享元模式`
- 基于`状态机`的HTTP协议解析器
 - 使用自己构建的最大堆关闭`超时连接`
- Linux Kernel 2.6+新特性(`ReusePort`)
-  为减少内存泄漏的可能,使用`智能指针`等RAII机制
- 使用多线程,epoll`边沿触发`的多路复用技术,非阻塞IO
- C++部分特性(`thread_local ` , `std::string_vew`...)
- 配置简单.使用`JSON`格式单配置文件,通过`rapidjson` 来进行解析,拥有初始配置文档,不需要修改大量配置文件.
- ...

# 性能
###  测试环境


  - CPU: Intel(R) Core(TM) i5-7200U CPU @ 2.50GHz
  -  内存: 12GiB
  -  网卡:	RealTek RTL-8169 Gigabit Ethernet driver 1000M
  - 操作系统: Manjaro Linux

  ### 测试工具


使用工具: `ab2.3`和`Webbench1.5`

### Ab2.3

单客户端测试
 
 测试命令:

`ab -n 500000 -c 1 http:127.0.0.1:<端口号>/`


完成时间(越少越好):


![image](https://github.com/LhdDream/my_web/blob/master/my_web/img/ab1.png)


每秒完成请求数(越多越好):

![image](https://github.com/LhdDream/my_web/blob/master/my_web/img/ab2.png)

每个请求花费的时间（越少越好）：

![image](https://github.com/LhdDream/my_web/blob/master/my_web/img/ab3.png)

百分比请求完成时间（越少越好）：


| 测试项目      | 90%(ms) | 99%(ms) | 100%(ms) |
| ------------- | ------- | ------- | -------- |
| nginx/1.16.1  | 0       | 0       | 40       |
| Apache/2.4.41 | 0       | 0       | 213      |
| My_web        | 0       | 0       | 10       |


# 多客户端测试

测试命令
`ab -n 500000 -c 520 http:127.0.0.1:<端口号>/
`

完成时间(越少越好):

![image](https://github.com/LhdDream/my_web/blob/master/my_web/img/ab4.png)

每秒完成请求数(越多越好):

![image](https://github.com/LhdDream/my_web/blob/master/my_web/img/ab5.png)

每个请求花费的时间（越少越好）：

![image](https://github.com/LhdDream/my_web/blob/master/my_web/img/ab6.png)

百分比请求完成时间（越少越好）：

| 测试项目      | 90%(ms) | 99%(ms) | 100%(ms) |
| ------------- | ------- | ------- | -------- |
| nginx/1.16.1  | 30      | 39      | 1247     |
| Apache/2.4.41 | 33      | 244     | 1475     |
| My_web        | 29      | 37      | 1045     |

# Webbenchv1.5


测试命令:

`./webbench http://127.0.0.1:<端口号>/ -2 --get -c <客户端数量>`

单客户端测试

`Nginx结果`


![image](https://github.com/LhdDream/my_web/blob/master/my_web/img/Web1a.png)


`My_Web结果`

![image](https://github.com/LhdDream/my_web/blob/master/my_web/img/Web2a.png)

10个客户端测试

`Nginx结果`


![image](https://github.com/LhdDream/my_web/blob/master/my_web/img/Web1b.png)


`My_Web结果`


![image](https://github.com/LhdDream/my_web/blob/master/my_web/img/Web2b.png)

3000个客户端测试


`Nginx结果`

![image](https://github.com/LhdDream/my_web/blob/master/my_web/img/Web1c.png)


`My_Web结果`


![image](https://github.com/LhdDream/my_web/blob/master/my_web/img/Web2c.png)

# 安装和运行

首先请保证您的计算机上安装了`cmake`>=3.15版本,可以使用软件源来进行安装


- Ubuntu  : sudo apt-get install  cmake 
- Centos: sudo yum install cmake 
- Fedora: sudo dnf install cmake 
- Manjaro: sudo pacman -S cmake

安装后，可能需要保证您的内核支持`SO_REUSEPORT`特性,虽然现在内核基本都支持,可能会有特殊，然后您可以通过`git`等方式进行下载
- 进入到相应的目录
- mkdir build
- cd build
- cmake ..
- make
- cd ../config
- sudo cp my_web.json /etc/my_web.json
- cd ../build
- ./my_web


# 配置说明


在`config`目录下有一个`my_web.json`文件，此文件为配置文件，您应该放在/etc/目录之下

my_web.json中的含义

- `server port`表示您所想绑定的端口
-  `sevrer threads`表示您想要创建的线程数
-  `serverip`表示您所想绑定的ip地址
- `server keep_connection_ms`表示超过多长时间就断开链接,单位为ms
- `server keep_connection_number` 连接服务器多少人数之后开始淘汰,一般建议不小于10
- `sites wwwroot` 表示默认目录位置
- `sites default_file`表示默认显示文件
- `Fastcgi`下为fastcgi的ip和端口

