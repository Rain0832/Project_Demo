#include <iostream>
#include <vector>
#include <mutex>

using namespace std;

class block
{
public:
  int size; // free: > 0, < 0
  int key;
  int data;
};

class blocks
{
private:
  vector<block> my_blocks;
  mutable mutex mtx;

  void getFreeBlock(block *freeBlock)
  {
    for (block t_block : my_blocks)
    {
      if (t_block.size > 0)
      {
        *freeBlock = t_block;
      }
    }
  }

  void m_put(int k, int v)
  {
    block *toPutBlock = nullptr;
    getFreeBlock(toPutBlock);
    if (toPutBlock == nullptr)
    {
      return;
    }
    toPutBlock->key = k;
    toPutBlock->data = v;
  }

  int m_get(int k) const
  {
    for (block t_block : my_blocks)
    {
      if (t_block.data == k)
      {
        return t_block.data;
      }
    }
    return -1;
  }

public:
  void put(int k, int v)
  {
    mtx.lock();
    m_put(k, v);
    mtx.unlock();
  }

  int get(int k) const
  {
    int data;
    mtx.lock();
    data = m_get(k);
    mtx.unlock();
    return data;
  }
};

int main()
{
}