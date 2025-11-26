#pragma

#include <iostream>
class Consumer
{
private:
  int &num;

public:
  Consumer(int &num) : num(num) {}
  void consume()
  {
    num--;
    std::cout << "Consumer: num = " << num << " ." << std::endl;
  }
};