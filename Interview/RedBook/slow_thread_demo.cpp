// slow_thread_demo.cpp
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <unistd.h>

std::mutex m;

void busy_worker(int id)
{
  while (true)
  {
    // try to lock, do a short work then unlock
    std::lock_guard<std::mutex> lk(m);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    // do some small work
    // print occasionally
    if (id == 1)
    { // let thread 1 print a heartbeat
      std::cout << "[worker " << id << "] heartbeat\n";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
}

void bad_thread()
{
  // This thread acquires the mutex and holds it for a long time,
  // simulating a stuck/slow thread that causes other threads to block.
  std::lock_guard<std::mutex> lk(m);
  std::cout << "[bad_thread] holding lock for 30s\n";
  std::this_thread::sleep_for(std::chrono::seconds(30));
  std::cout << "[bad_thread] releasing lock\n";
}

int main()
{
  std::cout << "PID: " << getpid() << "\n";
  std::vector<std::thread> t;
  // spawn several workers
  for (int i = 0; i < 4; ++i)
  {
    t.emplace_back(busy_worker, i);
  }
  // let them run a bit
  std::this_thread::sleep_for(std::chrono::seconds(1));
  // spawn the bad thread that will hold the lock
  std::thread bad(bad_thread);

  // join bad, detach workers
  bad.join();
  for (auto &th : t)
  {
    if (th.joinable())
      th.detach();
  }
  return 0;
}
