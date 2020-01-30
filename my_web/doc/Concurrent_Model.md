# 并发模型

程序使用Reactor模型，并使用多线程提高并发度.为避免线程频繁创建和销毁带来的开销,在程序的开始创建固定数量的线程.

如下图所示:


![image](https://github.com/LhdDream/my_web/blob/master/my_web/img/model.png)


每一个线程都有各自的epollfd和listenfd和各自的定时器,新建立的连接在UserSet类中进行注册,处理相应的事件
