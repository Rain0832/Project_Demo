#pragma once

#include "Buffer.hpp"
#include <chrono>
#include <thread>

class Consumer
{
public:
  Consumer(Buffer &buffer, int id) : buffer_(buffer), id_(id) {}
  void run()
  {
    while (true)
    {
      buffer_.consume(id_);
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
  }

private:
  Buffer &buffer_;
  uint id_;
};