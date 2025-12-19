// parallel_island.cpp
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <stack>
#include <chrono>

using Grid = std::vector<std::vector<int>>;
int rows, cols;

// helper to convert r,c to index
inline int idx(int r, int c) { return r * cols + c; }

int maxAreaParallel(const Grid &grid, int num_threads)
{
  rows = grid.size();
  cols = grid.empty() ? 0 : grid[0].size();
  if (rows == 0 || cols == 0)
    return 0;

  std::vector<std::atomic<char>> visited(rows * cols);
  for (auto &a : visited)
    a.store(0);

  std::vector<int> local_max(num_threads, 0);

  auto worker = [&](int tid, int start_row, int end_row)
  {
    int local_best = 0;
    // iterate starting points (row-major) but only rows in [start_row, end_row)
    for (int r = start_row; r < end_row; ++r)
    {
      for (int c = 0; c < cols; ++c)
      {
        if (grid[r][c] == 0)
          continue;
        int id0 = idx(r, c);
        char expected = 0;
        // try to mark visited atomically; only the thread that succeeds proceeds to explore this component
        if (!visited[id0].compare_exchange_strong(expected, 1))
          continue;
        // BFS/DFS using stack
        int area = 0;
        std::stack<int> st;
        st.push(id0);
        while (!st.empty())
        {
          int cur = st.top();
          st.pop();
          int cr = cur / cols, cc = cur % cols;
          ++area;
          // neighbors
          const int dr[4] = {-1, 1, 0, 0};
          const int dc[4] = {0, 0, -1, 1};
          for (int k = 0; k < 4; ++k)
          {
            int nr = cr + dr[k], nc = cc + dc[k];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols)
              continue;
            if (grid[nr][nc] == 0)
              continue;
            int nid = idx(nr, nc);
            char ex = 0;
            // attempt to claim the neighbor
            if (visited[nid].compare_exchange_strong(ex, 1))
            {
              st.push(nid);
            }
          }
        }
        if (area > local_best)
          local_best = area;
      }
    }
    local_max[tid] = local_best;
  };

  // compute partition rows per thread
  int chunk = (rows + num_threads - 1) / num_threads;
  std::vector<std::thread> ths;
  for (int t = 0; t < num_threads; ++t)
  {
    int sr = t * chunk;
    int er = std::min(rows, sr + chunk);
    if (sr >= er)
    {
      local_max[t] = 0;
      continue;
    }
    ths.emplace_back(worker, t, sr, er);
  }
  for (auto &t : ths)
    t.join();

  int best = 0;
  for (int v : local_max)
    if (v > best)
      best = v;
  return best;
}

int main()
{
  // simple test grid
  Grid grid = {
      {0, 1, 1, 0, 0, 1, 1, 1},
      {0, 1, 1, 0, 0, 1, 0, 0},
      {0, 0, 0, 0, 0, 1, 1, 0},
      {1, 1, 0, 1, 0, 0, 0, 0},
      {1, 1, 0, 1, 1, 0, 0, 0}};
  int num_threads = 4;
  auto t1 = std::chrono::steady_clock::now();
  int ans = maxAreaParallel(grid, num_threads);
  auto t2 = std::chrono::steady_clock::now();
  std::cout << "Max area = " << ans << "  (threads=" << num_threads << ")\n";
  std::cout << "Elapsed ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "\n";
  return 0;
}
