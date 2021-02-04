#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <cmath>

std::vector<int> temperatures;
std::vector<int> parent;
std::vector<bool> safe_spot;
std::vector<bool> memo;

int n, k, m;

void algo(int i) {
  if (memo[i]) return; // already been here
  std::queue<int> path; path.push(i);
  std::multiset<int> temps; temps.insert(temperatures[i]);
  
  memo[i] = true; // mark as visited
  
  int maxh = temperatures[i], minh = maxh, next = i;
  
  while (true) {
    int dh = maxh-minh;
    if (dh > k || (int)path.size() == m) { // stop trip
      memo[i] = true; // this index has been visited now
      if (dh <= k) safe_spot[next] = true; // is a safe spot if length of m within temperature range
      path.pop(); // remove the first in the queue
      temps.erase(temps.find(temperatures[i]));
      i = parent[i]; // advance to parent
      if (memo[i]) return; // return if already visited
    }
    next = parent[next];
    if (next < 0) {
      memo[i] = true; // declare as visited
      return;
    }
    path.push(next);
    temps.insert(temperatures[next]);
    auto it = temps.end(); it--;
    maxh = *it; minh = *temps.begin();
  }
}

void testcase() {
  std::cin >> n >> m >> k;
  
  temperatures = std::vector<int>(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> temperatures[i];
  }
  
  memo = std::vector<bool>(n, false);
  parent = std::vector<int>(n, -1);
  std::vector<bool> leaf(n, true);
  safe_spot = std::vector<bool>(n, false);
  
  for (int i = 0; i < n-1; ++i) {
    int u, v; std::cin >> u >> v;
    parent[v] = u;
    leaf[u] = false;
  }
  
  for (int i = 0; i < n; ++i) {
    if (leaf[i]) {
      algo(i);
    }
  }
  
  bool sol = false;
  for (int i = 0; i < n; ++i) {
    if(safe_spot[i]) {
      sol = true;
      std::cout << i << ' ';
    }
  }
  if (!sol) std::cout << "Abort mission\n";
  else std::cout << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}