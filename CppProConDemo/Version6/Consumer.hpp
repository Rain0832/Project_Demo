#include <iostream>
#include <mutex>
#include <thread>
#include "MPSCQueue.hpp"

template <typename T, size_t SIZE>
class Consumer
{
public:
  void run(MPSCQueue<T, SIZE> &q, size_t total, std::mutex &mtx)
  {
    size_t cnt = 0;
    while (cnt < total)
    {
      T val{};
      if (q.pop(val))
      {
        {
          std::lock_guard<std::mutex> lock(mtx);
          std::cout << "    [Consumer] " << val << std::endl;
        }
        ++cnt;
      }
      else
      {
        std::this_thread::yield();
      }
    }
  }
};