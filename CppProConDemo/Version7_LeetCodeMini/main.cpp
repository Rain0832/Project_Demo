#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <cstdio>
#include <mutex>
#include <thread>
#include <vector>

class BoundedBlockingQueue
{
public:
  explicit BoundedBlockingQueue(std::size_t cap) : capacity(cap) {}

  void enqueue(int val)
  {
    std::unique_lock<std::mutex> lk(mtx);
    cv_full.wait(lk, [this]
                 { return dq.size() < capacity; });
    dq.push_back(val);
    lk.unlock();
    cv_empty.notify_one();
  }

  int dequeue()
  {
    std::unique_lock<std::mutex> lk(mtx);
    cv_empty.wait(lk, [this]
                  { return !dq.empty(); });
    int v = dq.front();
    dq.pop_front();
    lk.unlock();
    cv_full.notify_one();
    return v;
  }

  std::size_t size() const
  {
    std::lock_guard<std::mutex> lk(mtx);
    return dq.size();
  }

private:
  std::deque<int> dq;
  mutable std::mutex mtx;
  std::condition_variable cv_empty, cv_full;
  std::size_t capacity;
};

/* ---------- 可视化 ---------- */
void monitor(BoundedBlockingQueue &q, std::atomic<bool> &done)
{
  while (!done)
  {
    std::size_t s = q.size();
    std::fprintf(stderr, "\r[");
    for (std::size_t i = 0; i < 10; ++i)
      std::fprintf(stderr, "%s", (i < s ? "█" : "░"));
    std::fprintf(stderr, "] %zu/10   ", s);
    std::fflush(stderr);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  std::fprintf(stderr, "\n");
}

/* ---------- 主线程 ---------- */
int main()
{
  BoundedBlockingQueue q(10);
  std::atomic<bool> done{false};

  std::thread mon(monitor, std::ref(q), std::ref(done));

  /* 3 生产者 */
  std::vector<std::thread> producers;
  for (int i = 0; i < 3; ++i)
    producers.emplace_back([&q, i]
                           {
            for (int j = 0; j < 15; ++j) {
                q.enqueue(i * 100 + j);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            } });

  /* 2 消费者 */
  std::vector<std::thread> consumers;
  for (int i = 0; i < 2; ++i)
    consumers.emplace_back([&q, i]
                           {
            for (int j = 0; j < 20; ++j) {
                int v = q.dequeue();
                std::fprintf(stderr, "C%d ← %d\n", i, v);
                std::fflush(stderr);          // 每条立刻刷新
            } });

  for (auto &t : producers)
    t.join();
  for (auto &t : consumers)
    t.join();

  std::this_thread::sleep_for(std::chrono::seconds(1)); // 让进度条多跑会儿
  done = true;
  mon.join();
  return 0;
}