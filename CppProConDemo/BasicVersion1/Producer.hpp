#pragma

#include <iostream>
class Producer
{
private:
  int &num;

public:
  Producer(int &num) : num(num) {}
  void produce()
  {
    num++;
    std::cout << "Producer: num = " << num << " ." << std::endl;
  }
};