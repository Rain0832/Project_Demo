#include <iostream>
#include <mutex>
#include <thread>
#include "RingBuffer.hpp"

template <typename T, size_t SIZE>
class Consumer
{
public:
  void run(RingBuffer<T, SIZE> &rb, size_t ConsumerNum, std::mutex &mtx)
  {
    for (size_t i = 0; i < ConsumerNum; ++i)
    {
      T item{};
      while (!rb.pop(item))
        std::this_thread::yield();
      {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "[Consumer] " << item << std::endl;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
};