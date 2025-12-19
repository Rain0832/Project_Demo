#include <iostream>

using namespace std;

int count()
{
  int n = 0;
  n++;
  return n;
}

int countWithStatic()
{
  static int n = 0;
  n++;
  return n;
}

int &countWithStaticRefer()
{
  static int x = 0;
  x++;
  return x;
}

int main()
{
  cout << "count() = " << count() << endl;

  cout << "countWithStatic() = " << countWithStatic() << endl;

  int &a = countWithStaticRefer();
  cout << "a = " << a << endl;
  cout << "countWithStaticRefer() = " << countWithStaticRefer() << endl;
  a++;
  cout << "a = " << a << endl;
  cout << "countWithStaticRefer() = " << countWithStaticRefer() << endl;
}