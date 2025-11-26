#pragma once

#include <condition_variable>
#include <iostream>
#include <mutex>

class Buffer
{
public:
  Buffer(int capacity) : buffer_(0), capacity_(capacity) {}

  void produce()
  {
    std::unique_lock<std::mutex> lock(mtx_);
    cond_not_full_.wait(lock, [&]()
                        { return buffer_ < capacity_; });
    buffer_++;
    std::cout << "[Producer] produced, buffer = " << buffer_ << std::endl;
    cond_not_empty_.notify_one();
  }

  void consume()
  {
    std::unique_lock<std::mutex> lock(mtx_);
    cond_not_empty_.wait(lock, [&]()
                         { return buffer_ > 0; });
    buffer_--;
    std::cout << "[Consumer] consumed, buffer = " << buffer_ << std::endl;
    cond_not_full_.notify_one();
  }

private:
  int buffer_;
  int capacity_;
  std::mutex mtx_;
  std::condition_variable cond_not_empty_;
  std::condition_variable cond_not_full_;
};