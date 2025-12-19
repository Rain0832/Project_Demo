// hash_demo.cpp
#include <bits/stdc++.h>
using namespace std;

// ---------- Separate chaining ----------
template <typename K, typename V>
struct ChainHash
{
  vector<vector<pair<K, V>>> buckets;
  size_t n;
  ChainHash(size_t m = 8) : buckets(m), n(0) {}
  size_t idx(const K &k) const { return std::hash<K>{}(k) % buckets.size(); }
  void insert(const K &k, const V &v)
  {
    auto &b = buckets[idx(k)];
    for (auto &p : b)
      if (p.first == k)
      {
        p.second = v;
        return;
      }
    b.emplace_back(k, v);
    n++;
    if (n > buckets.size() * 2)
      rehash(buckets.size() * 2);
  }
  bool find(const K &k, V &out) const
  {
    auto &b = buckets[idx(k)];
    for (auto &p : b)
      if (p.first == k)
      {
        out = p.second;
        return true;
      }
    return false;
  }
  void rehash(size_t m)
  {
    vector<vector<pair<K, V>>> nb(m);
    for (auto &b : buckets)
      for (auto &p : b)
      {
        nb[std::hash<K>{}(p.first) % m].push_back(p);
      }
    buckets.swap(nb);
  }
};

// ---------- Open addressing (linear probing) ----------
template <typename K, typename V>
struct LinearProbeHash
{
  enum SlotState
  {
    EMPTY,
    OCCUPIED,
    DELETED
  };
  struct Slot
  {
    K key;
    V val;
    SlotState st = EMPTY;
  };
  vector<Slot> table;
  size_t n;
  LinearProbeHash(size_t m = 8) : table(m), n(0) {}
  size_t idx(const K &k, size_t i = 0) const { return (std::hash<K>{}(k) + i) % table.size(); }
  void insert(const K &k, const V &v)
  {
    if ((n + 1) * 2 >= table.size())
      rehash(table.size() * 2);
    size_t i = 0;
    while (true)
    {
      size_t j = idx(k, i++);
      if (table[j].st != OCCUPIED)
      {
        table[j].key = k;
        table[j].val = v;
        table[j].st = OCCUPIED;
        ++n;
        return;
      }
      if (table[j].st == OCCUPIED && table[j].key == k)
      {
        table[j].val = v;
        return;
      }
    }
  }
  bool find(const K &k, V &out) const
  {
    size_t i = 0;
    while (i < table.size())
    {
      size_t j = idx(k, i++);
      if (table[j].st == EMPTY)
        return false;
      if (table[j].st == OCCUPIED && table[j].key == k)
      {
        out = table[j].val;
        return true;
      }
    }
    return false;
  }
  void rehash(size_t m)
  {
    vector<Slot> old = table;
    table.assign(m, Slot());
    n = 0;
    for (auto &s : old)
      if (s.st == OCCUPIED)
        insert(s.key, s.val);
  }
};

// ---------- Double hashing ----------
template <typename K, typename V>
struct DoubleHash
{
  vector<pair<K, V>> table;
  vector<int> state; // 0 empty, 1 occ, 2 deleted
  size_t n;
  DoubleHash(size_t m = 8) : table(m), state(m, 0), n(0) {}
  size_t h1(const K &k) const { return std::hash<K>{}(k) % table.size(); }
  size_t h2(const K &k) const { return 1 + (std::hash<K>{}(k) % (table.size() - 1)); }
  void insert(const K &k, const V &v)
  {
    if ((n + 1) * 2 >= table.size())
      rehash(table.size() * 2);
    size_t i = 0;
    while (true)
    {
      size_t j = (h1(k) + i * h2(k)) % table.size();
      if (state[j] != 1)
      {
        table[j] = {k, v};
        state[j] = 1;
        ++n;
        return;
      }
      if (state[j] == 1 && table[j].first == k)
      {
        table[j].second = v;
        return;
      }
      ++i;
    }
  }
  bool find(const K &k, V &out) const
  {
    size_t i = 0;
    while (i < table.size())
    {
      size_t j = (h1(k) + i * h2(k)) % table.size();
      if (state[j] == 0)
        return false;
      if (state[j] == 1 && table[j].first == k)
      {
        out = table[j].second;
        return true;
      }
      ++i;
    }
    return false;
  }
  void rehash(size_t m)
  {
    vector<pair<K, V>> oldT = table;
    vector<int> oldS = state;
    table.assign(m, {});
    state.assign(m, 0);
    n = 0;
    for (size_t i = 0; i < oldT.size(); ++i)
      if (oldS[i] == 1)
        insert(oldT[i].first, oldT[i].second);
  }
};

int main()
{
  ChainHash<int, string> ch;
  ch.insert(1, "one");
  ch.insert(9, "nine");
  string v;
  cout << "Chain find 9: " << (ch.find(9, v) ? v : "-") << "\n";

  LinearProbeHash<int, string> lh;
  lh.insert(1, "one");
  lh.insert(9, "nine");
  cout << "Linear find 9: " << (lh.find(9, v) ? v : "-") << "\n";

  DoubleHash<int, string> dh;
  dh.insert(1, "one");
  dh.insert(9, "nine");
  cout << "Double find 9: " << (dh.find(9, v) ? v : "-") << "\n";
  return 0;
}
