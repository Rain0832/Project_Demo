#include <thread>
#include "RingBuffer.hpp"
#include "Consumer.hpp"
#include "Producer.hpp"

const size_t ProducerNum = 100;
const size_t ConsumerNum = 100;

int main()
{
  RingBuffer<int, 10> rbuffer;
  Producer<int, 10> producer;
  Consumer<int, 10> consumer;

  std::mutex mtx;
  std::thread prodThread(&Producer<int, 10>::run, &producer, std::ref(rbuffer), ProducerNum, std::ref(mtx));
  std::thread consThread(&Consumer<int, 10>::run, &consumer, std::ref(rbuffer), ConsumerNum, std::ref(mtx));

  prodThread.join();
  consThread.join();
};