#include <iostream>
#include <queue>
#include <vector>

void testcase() {
  int n, m, v; std::cin >> n >> m >> v;
  std::vector<std::vector<int>> neighbours(n);
  std::vector<int> distances(n, -1);
  
  for (int i = 0; i < m; ++i) {
    int a, b; std::cin >> a >> b;
    neighbours[a].push_back(b);
    neighbours[b].push_back(a);
  }
  
  distances[v] = 0; // starting vertex
  std::queue<int> q; q.push(v);
  
  while (!q.empty()) {
    int curr = q.front();
    for (int nb : neighbours[curr]) {
      // add all unvisited neighbours to the queue & set their distance
      if (distances[nb] == -1) {
        q.push(nb);
        distances[nb] = distances[curr] + 1;
      }
    }
    q.pop();
  }
  
  for (int d : distances) {
    std::cout << d << " ";
  }
  std::cout << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}