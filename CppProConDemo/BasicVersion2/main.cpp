#include "Consumer.hpp"
#include "Producer.hpp"
#include <mutex>

int main()
{
  int buffer = 5;
  const int capcity = 10;
  std::mutex mtx;
  Producer prod(buffer, capcity, mtx);
  Consumer cons(buffer, mtx);

  for (int i = 0; i < 10; i++)
  {
    prod.produce();
    cons.consume();
    prod.produce();
  }

  return 0;
}