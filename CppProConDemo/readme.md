# Cpp Pro-Con Demo

**A step-by-step journey from naive producer–consumer → locks → condition variables → lock-free ring buffer → MPSC queue**

本仓库记录了一个**从零开始构建并发基础设施**的实验路线。
通过六个渐进式版本，依次掌握：

* 多线程基础
* 互斥锁与条件变量
* 有界缓冲区设计
* 无锁环形队列（SPSC）
* 无锁多生产者单消费者队列（MPSC）

整个项目非常适合作为：

* **C++ 并发入门的系统训练**
* **网络服务器开发的并发底座学习**
* **面试时展示对锁 / 无锁的理解**

---

# 📂 目录结构（简述每个阶段的目标）

```
.
├── BasicVersion1     # 阶段 1：最小化 Producer–Consumer（单线程原型）
├── BasicVersion2     # 阶段 2：加入互斥锁（mutex）保护共享数据
├── ProVersion3       # 阶段 3：多线程 + 条件变量（经典 PC 模型）
├── Version4          # 阶段 4：多生产者 + 多消费者（正确使用锁 + CV）
├── Version5          # 阶段 5：无锁环形队列（SPSC Ring Buffer）
├── Version6          # 阶段 6：无锁 MPSC 队列（多生产者单消费者）
└── readme.md
```

---

# 🧭 学习路线总览

下面是整个实验的进阶逻辑，每一层都建立在上一层之上。

---

## **1. BasicVersion1 — 最简单的生产者/消费者模型（单线程）**

目标：

* 理解“共享变量 + 操作”的基本结构
* 明确 Producer / Consumer 的角色与行为
* 无锁、无多线程，只是一个概念原型

---

## **2. BasicVersion2 — 引入互斥锁（mutex）**

目标：

* 多线程环境下共享数据需要保护
* 使用 `std::mutex` 和 `std::lock_guard`
* 避免竞态条件 race condition

此版本首次使用多线程，但没有条件变量，因此：

* 不能阻塞等待
* 只能 busy-loop（效率低）

---

## **3. ProVersion3 — 使用条件变量（Condition Variable）**

目标：

* 构建真正的“生产者–消费者”模型
* 共享队列 + mutex + condition variable
* 正确使用：wait() / notify_one() / notify_all()

本版本是所有经典教材的“标准 PC 解决方案”。

---

## **4. Version4 — 多生产者 + 多消费者（MPMC with locks）**

目标：

* 扩展到多个生产者线程 + 多个消费者线程
* 深入理解锁竞争、虚假唤醒、队列空/满的判断
* 避免死锁、避免冗余唤醒

这是真实系统会使用的版本（如线程池工作队列）。

---

## **5. Version5 — 无锁环形队列（SPSC：单生产者单消费者）**

目标：

* 去掉锁，性能提升一个数量级
* 环形队列的原理（head/tail 指针）
* 内存顺序与 `std::atomic`（release/acquire）
* 学习为什么 SPSC 可以做到无锁

这是网络 I/O 库、音视频 pipeline、日志系统常用组件。

---

## **6. Version6 — 无锁多生产者单消费者队列（MPSC）**

目标：

* 真正进入无锁并发的“竞技场”
* 多生产者竞争写入位置 → CAS
* 每个 slot 使用 sequence number 避免 ABA
* 简化版本的 Dmitry Vyukov MPMC 算法

这是日志系统、消息循环、网络服务器内部队列最常见的模型。

学习到这一层，并发难度已经到达“工程级”。

---

# 🧪 如何编译与运行

进入任意版本目录，执行：

```bash
make
./main
```

对于多线程版本，可尝试打开多个终端观察输出时序，用 `htop` 查看 CPU 占用，或调整线程数观察行为变化。

---

# 🔍 如果想继续深入（推荐路线）

1. **Version6 → MPMC（多生产者多消费者）**
   最终 Boss，需要双向 CAS 和更复杂的状态机。

2. **加入 false sharing padding 优化**
   用 `alignas(64)` 为 head/tail/slots 进行 cache line 对齐。

3. **把 Version6 用进自己的 Linux 网络服务器里**

   * 网络线程 push I/O 事件
   * loop 线程 pop 并 epoll 处理
     ——这是 real-world 架构。

4. **用 benchmark 测性能（Google Benchmark）**
   对比：

   * mutex + queue
   * condition variable
   * SPSC lock-free
   * MPSC lock-free

---

# 🎯 项目目标（一句话）

> **通过六个循序渐进的小实验，从零走到能自己实现一个高性能无锁队列，为后续网络服务器开发打下坚实基础。**

---