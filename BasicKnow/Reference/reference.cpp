#include <iostream>

using namespace std;

int main()
{
  int a = 10;
  // 1. init
  int *p = &a;

  // 2. or defer assign
  // int *p = nullptr;
  // p = &a;

  int *&q = p;
  cout << "&a = " << &a << endl;
  cout << "p = " << p << endl;
  cout << "q = " << q << endl;
}