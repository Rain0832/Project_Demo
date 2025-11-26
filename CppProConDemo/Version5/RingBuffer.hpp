#pragma once

#include <atomic>
#include <cstddef>

template <typename T, size_t SIZE>
class RingBuffer
{
public:
  RingBuffer() : head_(0), tail_(0) {}

  bool push(const T &item)
  {
    size_t t = tail_.load(std::memory_order_relaxed);
    size_t next = (t + 1) % SIZE;

    // buffer_ is full
    if (next == head_.load(std::memory_order_acquire))
      return false;

    buffer_[t] = item;
    tail_.store(next, std::memory_order_release);
    return true;
  }

  bool pop(T &item)
  {
    size_t h = head_.load(std::memory_order_relaxed);

    // buffer_ is empty
    if (h == tail_.load(std::memory_order_acquire))
      return false;

    item = buffer_[h];
    head_.store((h + 1) % SIZE, std::memory_order_release);
    return true;
  }

private:
  T buffer_[SIZE];
  std::atomic<size_t> head_; // consumer read
  std::atomic<size_t> tail_; // producer write
};