# Multithreading and Thread Synchrony

From Bilibili: [Multithreading and Thread Synchronization-C/C++](https://www.bilibili.com/video/BV1sv41177e4?vd_source=630bda5b6aa6fe12cf2bbd8569afd13e)

It is recommended to use it with Gitbook: http://rain0832.gitbook.io/rain0832/bilibili-wild/multithreading-and-thread-synchrony

I define it as an introduction to operating systems and practice.

The most important understanding of operating systems is processes/threads, lock mechanisms, etc.

# 1. Multithreading features

# 2. Thread creation, exit, and recycling methods

# 3. Other thread functions

# 4. Thread synchronization processing ideas

# 5. Mutex locks

# 6. Thread deadlocks

# 7. Read-write locks

# 8. Conditional variables

# 9. Semaphores

# 10. Summary

The focus of learning operating systems is to understand how the system's threads operate. Threads have led to the concepts of multithreading and thread synchronization, which is the main content of this copy. In addition, the knowledge of system memory is also involved. This time the code is implemented in C language, and 80% of the code is hand-written. In addition, I took this opportunity to reuse the Ubuntu system for programming. This time, I used the vim editor for programming throughout the process. In the process of asking Kimi again and again, I forgot again and again, and with the help of AI, my programming ability has grown rapidly.

This can only be considered a brief introduction, and it cannot even be considered an introductory course on operating systems. It can only be considered an introductory course on threads. There is still a lot of knowledge and practical experience about operating systems and threads. These hundreds of lines of code are far from enough. You can only have a general understanding, but this is enough.

---

# 多线程与线程同步

来自B站：[爱编程的大丙](【多线程和线程同步-C/C++】https://www.bilibili.com/video/BV1sv41177e4?vd_source=630bda5b6aa6fe12cf2bbd8569afd13e)

建议与Gitbook搭配食用：http://rain0832.gitbook.io/rain0832/bilibili-wild/multithreading-and-thread-synchrony

我将其定义为操作系统导论与实践。

操作系统最重要的理解就是进程/线程，锁机制等。

# 1. 多线程特点

# 2. 线程的创建、退出、回收方法

# 3. 其他线程函数

# 4. 线程同步处理思路

# 5. 互斥锁

# 6. 线程死锁

# 7. 读写锁

# 8. 条件变量

# 9. 信号量

# 10. 总结

操作系统的学习重点就在于理解系统的线程如何操作，线程又引申出了多线程与线程同步的概念，也就是这次副本的主要内容。另外也涉及到了系统内存的知识，这次的代码为C语言实现，80%的代码是手搓的。另外借此机会重新复用了Ubuntu系统进行编程。这次全程使用vim编辑器进行编程。在一次次的问kimi的过程中，一次次的又忘记，借助AI让自己的编程能力快速成长。

这次只能算是浅尝辄止，甚至不能算作操作系统的导论课，只能算作线程的导论课，关于操作系统和线程都还有很多知识和实践经验，单靠这几百行代码远远不够，只能是了解个大概，但这也够了。