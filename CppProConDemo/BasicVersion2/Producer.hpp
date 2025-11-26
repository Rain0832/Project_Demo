#pragma

#include <iostream>
#include <mutex>

class Producer
{
private:
  int &buffer;
  int capcity;
  std::mutex &mutex;

public:
  Producer(int &buf, int cap, std::mutex &mtx) : buffer(buf), capcity(cap), mutex(mtx) {}
  void produce()
  {
    std::lock_guard<std::mutex> lock(mutex);

    if (buffer < capcity)
    {
      buffer++;
      std::cout << "[Producer] produced one. buffer = " << buffer << std::endl;
    }
    else
    {
      std::cout << "[Producer] buffer FULL, cannot produce." << std::endl;
    }
  }
};