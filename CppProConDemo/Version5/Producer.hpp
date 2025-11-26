#include <iostream>
#include <mutex>
#include <thread>
#include "RingBuffer.hpp"

template <typename T, size_t SIZE>
class Producer
{
public:
  void run(RingBuffer<T, SIZE> &rb, size_t ProducerNum, std::mutex &mtx)
  {
    for (int i = 1; i <= ProducerNum; i++)
    {
      while (!rb.push(i))
        std::this_thread::yield();
      {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "[Producer] " << i << std::endl;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }
};