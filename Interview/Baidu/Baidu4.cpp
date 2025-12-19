#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

class Lru
{
private:
  unordered_map<int, list<pair<int, int>>::iterator> u_map;
  list<pair<int, int>> m_list;
  int capacity;

public:
  Lru(int cap) : capacity(cap) {}

  void put(int k, int v)
  {
    auto it = u_map.find(k);
    if (it != u_map.end())
    {
      it->second->second = v;
      m_list.splice(m_list.begin(), m_list, it->second);
    }
    m_list.push_front({k, v});
    u_map[k] = m_list.begin();
    if (capacity < m_list.size())
    {
      u_map.erase(m_list.back().first);
      m_list.pop_back();
    }
  }

  int get(int k)
  {
    auto it = u_map.find(k);
    if (it == u_map.end())
      return -1;
    m_list.splice(m_list.begin(), m_list, it->second);
    int res = it->second->second;
  }
};

int main()
{
  Lru m_lru(3);
  m_lru.put(1, 1);
  m_lru.put(2, 4);
  m_lru.put(3, 9);
  std::cout << m_lru.get(1) << std::endl;
  std::cout << m_lru.get(2) << std::endl;
  std::cout << m_lru.get(3) << std::endl;
  m_lru.put(4, 16);
  std::cout << m_lru.get(1) << std::endl;

  return 0;
}