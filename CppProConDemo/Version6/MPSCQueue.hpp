#pragma once

#include <atomic>
#include <cstddef>

template <typename T, size_t SIZE>
class MPSCQueue
{
public:
  MPSCQueue()
  {
    for (size_t i = 0; i < SIZE; i++)
      buffer_[i].seq.store(i, std::memory_order_relaxed);
    head_.store(0);
    tail_.store(0);
  }

  bool push(const T &item)
  {
    size_t t = tail_.load(std::memory_order_relaxed);

    while (true)
    {
      Slot &slot = buffer_[t & MASK];
      size_t seq = slot.seq.load(std::memory_order_acquire);

      intptr_t diff = (intptr_t)seq - (intptr_t)t;

      if (diff == 0)
      {
        // 尝试占用该 slot：CAS 抢写
        if (tail_.compare_exchange_weak(
                t, t + 1,
                std::memory_order_relaxed,
                std::memory_order_relaxed))
        {
          // 真正写入数据
          slot.data = item;

          // 完成写入，seq 增加 1：表示“可读”
          slot.seq.store(t + 1, std::memory_order_release);
          return true;
        }
      }
      else if (diff < 0)
      {
        // diff < 0 表示“队列满”
        return false;
      }
      else if (diff > 0)
      {
        // 别的线程写成功了，推进 t 重试
        t = tail_.load(std::memory_order_relaxed);
      }
    }
  }

  bool pop(T &item)
  {
    size_t h = head_.load(std::memory_order_relaxed);
    Slot &slot = buffer_[h & MASK];

    size_t seq = slot.seq.load(std::memory_order_acquire);

    intptr_t diff = (intptr_t)seq - (intptr_t)(h + 1);

    if (diff == 0)
    {
      // slot 可读
      item = slot.data;

      // 读完后重置 seq 到下一轮初始时代码：h + SIZE （避免 ABA）
      slot.seq.store(h + SIZE, std::memory_order_release);

      head_.store(h + 1, std::memory_order_relaxed);
      return true;
    }
    return false; // empty
  }

private:
  struct Slot
  {
    std::atomic<size_t> seq;
    T data;
  };
  static constexpr size_t MASK = SIZE - 1;
  Slot buffer_[SIZE];
  std::atomic<size_t> head_; // consumer read
  std::atomic<size_t> tail_; // producer write
};