#include "Consumer.hpp"
#include "Producer.hpp"

int main()
{
  int num = 0;
  Consumer cons(num);
  Producer prod(num);

  // num = 0
  cons.consume(); // = -1
  prod.produce(); // = 0
  cons.consume(); // = -1
  cons.consume(); // = -2
  prod.produce(); // = -1
  prod.produce(); // = 0

  return 0;
}