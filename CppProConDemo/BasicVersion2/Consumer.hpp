#pragma

#include <iostream>
#include <mutex>

class Consumer
{
private:
  int &buffer;
  std::mutex &mutex;

public:
  Consumer(int &buf, std::mutex &mtx) : buffer(buf), mutex(mtx) {}
  void consume()
  {
    std::lock_guard<std::mutex> lock(mutex);

    if (buffer > 0)
    {
      buffer--;
      std::cout << "[Consumer] consumed one. buffer = " << buffer << std::endl;
    }
    else
    {
      std::cout << "[Consumer] buffer EMPTY, cannot consume." << std::endl;
    }
  }
};