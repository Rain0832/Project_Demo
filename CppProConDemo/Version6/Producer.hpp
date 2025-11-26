#include <iostream>
#include <mutex>
#include <thread>
#include "MPSCQueue.hpp"

template <typename T, size_t SIZE>
class Producer
{
public:
  void run(MPSCQueue<T, SIZE> &q, size_t perThread, std::mutex &mtx)
  {
    for (size_t i = 0; i < perThread; ++i)
    {
      T val = T(i + 1);    // 本地序号
      while (!q.push(val)) // CAS 抢写
        std::this_thread::yield();
      {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "[Producer-" << std::this_thread::get_id() << "] "
                  << val << std::endl;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }
};