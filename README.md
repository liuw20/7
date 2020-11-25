# 7
将原先的父子间进程操作改成使用进程间通信机制  
csv2bin函数将文件分成三行，分发进程进程进行转码  
bincat函数使用mmap文件映射方式读取二进制文件  
![Image text](https://github.com/liuw20/7/blob/master/2020-11-20%2020-07-50%20%E7%9A%84%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE.png)

测试流程：  
1、gcc编译7_new.c  
2、执行编译文件即可  
