#include <vector>

#include "Buffer.hpp"
#include "Consumer.hpp"
#include "Producer.hpp"

int main()
{
  Buffer buffer(10);
  const int producerNum = 6;
  const int consumerNum = 7;

  std::vector<std::thread> threads;

  for (int i = 0; i < producerNum; i++)
  {
    threads.emplace_back([&, i]()
                         { Producer prod(buffer, i);
                        prod.run(); });
  }

  for (int i = 0; i < consumerNum; i++)
  {
    threads.emplace_back([&, i]()
                         { Consumer cons(buffer, i);
                        cons.run(); });
  }

  for (auto &t : threads)
    t.join();

  return 0;
}