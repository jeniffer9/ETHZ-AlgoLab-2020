#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

void testcase() {
  int n, m; std::cin >> n >> m;
  std::vector<int> strength(n);
  std::vector<int> boxes(m);
  
  for (int i = 0; i < n; ++i) {
    std::cin >> strength[i];
  }
  for (int i = 0; i < m; ++i) {
    std::cin >> boxes[i];
  }
  
  std::sort(strength.begin(), strength.end(), std::greater<int>());
  
  // sort it that way in order to use std::lower_bound
  std::sort(boxes.begin(), boxes.end(), std::greater<int>());
  
  if (boxes[0] > strength[0]) {
    std::cout << "impossible\n";
    return;
  }
  
  std::multiset<int, std::greater<int>> bxs(boxes.begin(), boxes.end());
  
  int r = 0;
  while (!bxs.empty()) {
    for (int i = 0; i < n; ++i) { // go through each friend and take biggest box
      auto lb = bxs.lower_bound(strength[i]);
      if (lb == bxs.end()) {
        break;
      } else {
        bxs.erase(lb);
      }
    }
    r++; // another round
  }
  
  std::cout << r*3-1 << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
}