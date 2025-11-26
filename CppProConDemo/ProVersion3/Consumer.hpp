#pragma once

#include "Buffer.hpp"
#include <chrono>
#include <thread>

class Consumer
{
public:
  Consumer(Buffer &buffer) : buffer_(buffer) {}
  void run()
  {
    while (true)
    {
      buffer_.consume();
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
  }

private:
  Buffer &buffer_;
};