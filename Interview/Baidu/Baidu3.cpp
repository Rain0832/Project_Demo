#include <mutex>
#include <iostream>
#include <thread>

using namespace std;

mutex mtx1;
mutex mtx2;

void thread1()
{
  mtx1.lock();
  cout << "1 Got mtx1" << endl;
  mtx2.lock();
  cout << "1 Got mtx2" << endl;
  mtx2.unlock();
  mtx1.unlock();
}

void thread2()
{
  mtx2.lock();
  cout << "2 Got mtx2" << endl;
  mtx1.lock();
  cout << "2 Got mtx1" << endl;
  mtx1.unlock();
  mtx2.unlock();
}

int main()
{
  int cnt = 0;
  while (1)
  {
    cout << cnt << " Round" << endl;
    thread t1(thread1);
    thread t2(thread2);

    t1.join();
    t2.join();
    cnt++;
  }
}