# 并发模型

程序使用Reactor模型，并使用多线程提高并发度.为避免线程频繁创建和销毁带来的开销,在程序的开始创建固定数量的线程.

如下图所示:
[](https://github.com/LhdDream/my_web/blob/master/my_web/img/model.png)
