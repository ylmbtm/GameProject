GameProject
===========

A MMOG Game Engine

email : ylmbtm@163.com
QQ    : 191342599

介绍
1.一个基于高效, 简洁, 使用方便,跨平台的服务器网引擎, 以及一个简单多进程的简单服务器框架。

服务器网引擎将连接的处理,数据包的收发等细节隐藏, 让网络层对于逻辑层来说完全透明。

引擎同时支持windows(IOCP)和linux(EPOLL)两个系统,采用纯Socket API进行开发,未基于ASIO,libevent,ACE之类的第三方库, 代码比

较简单, 易于调试跟踪。

引擎采用了无锁对列,内存池,对像池等多种手段提升性能,和内存使用效率并尽可能减少内存碎片产生。
引擎还提供内存池,对象池,无锁队列,还专门实现了一个基于AVL Tree的map容器, 内存管理更清晰,复用性更好.

特点
1.使用简单的对象池(MemObjectPool)

2.内存更高效的AVLTree
  一个内存更新高的map容器， 实现了map容器的查找，添加，删除，遍历，内存更完整，不产生内存碎片。

3.高效的无锁队列(简单改编自第三方代码)
  用来管理消息队列, 由于无锁, 多线程访问性能更好。

4.本项目是一个单进程多业务线程的方案.(在GameProject2里实现的是单进程单线程的方案)。



https://github.com/ylmbtm/GameProject/
GameProject  实现的是一个单进程多线程的方案。

https://github.com/ylmbtm/GameProject2/
GameProject2  实现了流行的单进程单线程的方案。

https://github.com/ylmbtm/TestTransfer/
为了避免使用protobuff,  本人写一个小工具， 支持把一个自定义消息格式(直接使用的c的struct定义语法), 转成
C#语言、GO语言、lua语言、C++语言 的包解析结构, 可以实现不同语言间的网络通信, 但不用包含protobuf库中那么多的文件,
比较轻量级,非常简单,文件少, 无需包含第三方库文件。
