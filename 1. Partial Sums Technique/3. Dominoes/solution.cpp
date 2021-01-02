#include <iostream>
#include <algorithm>
#include <vector>

void testcase() {
  int n; std::cin >> n;
  std::vector<int> heights(n);
  
  for (int i =0; i < n; ++i) {
    std::cin >> heights[i];
  }
  
  int fallen = 1;
  int left = heights[0]; // how much height is left
  for (int i = 1; i < n && left > 1; ++i) { // height must be bigger than 1 to cause neighbour to fall
    ++fallen;
    left = std::max(left-1, heights[i]);
  }
  
  std::cout << fallen << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}