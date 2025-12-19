// sorts_demo.cpp
#include <bits/stdc++.h>
using namespace std;

void quickSort(vector<int> &a, int l, int r)
{
  if (l >= r)
    return;
  int i = l, j = r, pivot = a[(l + r) / 2];
  while (i <= j)
  {
    while (a[i] < pivot)
      ++i;
    while (a[j] > pivot)
      --j;
    if (i <= j)
      swap(a[i++], a[j--]);
  }
  if (l < j)
    quickSort(a, l, j);
  if (i < r)
    quickSort(a, i, r);
}

void mergeSort(vector<int> &a)
{
  int n = a.size();
  if (n <= 1)
    return;
  vector<int> tmp(n);
  function<void(int, int)> ms = [&](int l, int r)
  {
    if (r - l <= 0)
      return;
    int m = (l + r) / 2;
    ms(l, m);
    ms(m + 1, r);
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r)
      tmp[k++] = (a[i] <= a[j] ? a[i++] : a[j++]);
    while (i <= m)
      tmp[k++] = a[i++];
    while (j <= r)
      tmp[k++] = a[j++];
    for (int t = l; t <= r; ++t)
      a[t] = tmp[t];
  };
  ms(0, n - 1);
}

void heapSort(vector<int> &a)
{
  make_heap(a.begin(), a.end());
  vector<int> out;
  while (!a.empty())
  {
    pop_heap(a.begin(), a.end());
    out.push_back(a.back());
    a.pop_back();
  }
  reverse(out.begin(), out.end());
  a = move(out);
}

void bubbleSort(vector<int> &a)
{
  int n = a.size();
  for (int i = 0; i < n; i++)
  {
    bool swapped = false;
    for (int j = 1; j < n - i; j++)
    {
      if (a[j - 1] > a[j])
      {
        swap(a[j - 1], a[j]);
        swapped = true;
      }
    }
    if (!swapped)
      break;
  }
}

void insertSort(vector<int> &a)
{
  int n = a.size();
  for (int i = 1; i < n; i++)
  {
    int key = a[i], j = i - 1;
    while (j >= 0 && a[j] > key)
    {
      a[j + 1] = a[j];
      --j;
    }
    a[j + 1] = key;
  }
}

void selectSort(vector<int> &a)
{
  int n = a.size();
  for (int i = 0; i < n - 1; i++)
  {
    int min_i = i;
    for (int j = i + 1; j < n; j++)
      if (a[j] < a[min_i])
        min_i = j;
    swap(a[i], a[min_i]);
  }
}

void shellSort(vector<int> &a)
{
  int n = a.size();
  for (int gap = n / 2; gap > 0; gap /= 2)
  {
    for (int i = gap; i < n; i++)
    {
      int tmp = a[i], j = i;
      while (j >= gap && a[j - gap] > tmp)
      {
        a[j] = a[j - gap];
        j -= gap;
      }
      a[j] = tmp;
    }
  }
}

void bucketSort(vector<int> &a)
{
  if (a.empty())
    return;
  int n = a.size();
  int minv = *min_element(a.begin(), a.end()), maxv = *max_element(a.begin(), a.end());
  int bucketCount = max(1, (maxv - minv) / max(1, n / 10) + 1);
  vector<vector<int>> buckets(bucketCount);
  for (int v : a)
  {
    int idx = ((v - minv) * (bucketCount - 1)) / max(1, maxv - minv);
    buckets[idx].push_back(v);
  }
  for (auto &b : buckets)
  {
    sort(b.begin(), b.end()); // stable if sort is stable; here std::sort is not stable, but per-problem bucket internal stable algorithm can be used
  }
  int k = 0;
  for (auto &b : buckets)
    for (int v : b)
      a[k++] = v;
}

void printArr(const vector<int> &a)
{
  for (int x : a)
    cout << x << " ";
  cout << "\n";
}

int main()
{
  vector<int> base = {5, 3, 8, 4, 2, 7, 1, 10, 9, 6};

  {
    auto a = base;
    quickSort(a, 0, a.size() - 1);
    cout << "Quick: ";
    printArr(a);
  }

  {
    auto a = base;
    mergeSort(a);
    cout << "Merge: ";
    printArr(a);
  }

  {
    auto a = base;
    heapSort(a);
    cout << "Heap: ";
    printArr(a);
  }

  {
    auto a = base;
    bubbleSort(a);
    cout << "Bubble: ";
    printArr(a);
  }

  {
    auto a = base;
    insertSort(a);
    cout << "Insert: ";
    printArr(a);
  }

  {
    auto a = base;
    selectSort(a);
    cout << "Select: ";
    printArr(a);
  }

  {
    auto a = base;
    shellSort(a);
    cout << "Shell: ";
    printArr(a);
  }

  {
    auto a = base;
    bucketSort(a);
    cout << "Bucket: ";
    printArr(a);
  }
  return 0;
}
