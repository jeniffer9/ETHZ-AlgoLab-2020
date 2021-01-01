#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

void testcase() {
  int n, m, v; std::cin >> n >> m >> v;
  std::vector<std::vector<int>> neighbours(n);
  std::vector<int> discovery_times(n, -1);
  std::vector<int> finishing_times(n, -1);

  for (int i = 0; i < m; ++i) {
    int a, b; std::cin >> a >> b; // edge
    neighbours[a].push_back(b);
    neighbours[b].push_back(a);
  }
  
  for (int i = 0; i < n; ++i) { // sort by labels
    std::sort(neighbours[i].begin(), neighbours[i].end());
  }
  
  int t = 0;
  discovery_times[v] = t; // starting vertex
  std::stack<int> s; s.push(v);
  std::vector<int> next_neighbour(n, 0); // keep index of next neighbour to visit
  
  while (!s.empty()) {
    int curr_v = s.top();
    int to_visit_i = next_neighbour[curr_v]; // index
    if ((to_visit_i+1) > (int)neighbours[curr_v].size()) { // if visited all neighbours
      finishing_times[curr_v] = ++t;
      s.pop(); // remove from stack
    } else { // else visit next neighbour if unvisited and put it onto stack
      int to_visit = neighbours[curr_v][to_visit_i];
      if (discovery_times[to_visit] < 0) { // only if unvisited
        discovery_times[to_visit] = ++t;
        s.push(to_visit); // put onto stack
      }
      next_neighbour[curr_v]++; // increase index of next neighbour to visit
    }
  }
  
  for (auto dt : discovery_times) {
    std::cout << dt << " ";
  }
  std::cout << '\n';
  for (auto ft : finishing_times) {
    std::cout << ft << " ";
  }
  std::cout << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}