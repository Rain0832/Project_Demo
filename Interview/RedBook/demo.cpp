#include <iostream>
#include <thread>
#include <atomic>
#include <cassert>

int data = 0;
std::atomic<int> flag{0};

// 错误用法：可能导致 consumer 看不到 data = 42
void producer_bad()
{
  data = 42;                                // ① 写数据
  flag.store(1, std::memory_order_relaxed); // ② 不保证顺序
}

void consumer_bad()
{
  while (flag.load(std::memory_order_relaxed) == 0)
  {
  }                   // ③
  assert(data == 42); // ④ ❌ 可能失败！
}

// 正确用法：Release-Acquire 保证可见性
void producer_good()
{
  data = 42;                                // ① 写数据（release 前）
  flag.store(1, std::memory_order_release); // ② ✨ 发布数据
}

void consumer_good()
{
  while (flag.load(std::memory_order_acquire) == 0)
  {
  }                   // ③ ✨ 获取数据
  assert(data == 42); // ④ ✅ 必然成功
}

int main()
{
  // 测试 10000 次，relaxed 可能偶尔出错（取决于 CPU/编译器优化）
  for (int i = 0; i < 10000; ++i)
  {
    data = 0;
    flag = 0;

    std::thread t1(consumer_good);
    std::thread t2(producer_good);

    t1.join();
    t2.join();
  }
  std::cout << "Release-Acquire 测试通过 10000 次！\n";
  return 0;
}
