// main.cpp (bad)
#include <iostream>

// Note: no extern "C" here -> C++ name mangling expected
// int foo(); // declaration without extern "C"
int foo(); // declaration with extern "C"

int main()
{
  std::cout << "foo() = " << foo() << std::endl;
  return 0;
}
