#include "Consumer.hpp"
#include "Buffer.hpp"
#include "Producer.hpp"

int main()
{
  Buffer buffer(5);

  Producer prod(buffer);
  Consumer cons(buffer);

  std::thread prodThread1(&Producer::run, &prod);
  std::thread consThread1(&Consumer::run, &cons);
  std::thread prodThread2(&Producer::run, &prod);

  prodThread1.join();
  consThread1.join();
  prodThread2.join();

  return 0;
}