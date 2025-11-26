#include <thread>
#include <vector>
#include "MPSCQueue.hpp"
#include "Consumer.hpp"
#include "Producer.hpp"

constexpr size_t SIZE = 8;
constexpr size_t PROD_THREADS = 3;
constexpr size_t PER_PROD = 10; // 每个生产者写 10 个
constexpr size_t TOTAL = PROD_THREADS * PER_PROD;

int main()
{
  MPSCQueue<int, SIZE> q;
  std::mutex mtx;

  std::vector<std::thread> producers;
  for (size_t i = 0; i < PROD_THREADS; ++i)
    producers.emplace_back([&q, &mtx, i]()
                           {
            Producer<int,SIZE> p;
            p.run(q, PER_PROD, mtx); });

  std::thread consumer([&]()
                       {
        Consumer<int,SIZE> c;
        c.run(q, TOTAL, mtx); });

  for (auto &t : producers)
    t.join();
  consumer.join();
};