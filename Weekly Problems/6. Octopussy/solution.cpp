#include <iostream>
#include <vector>
#include <algorithm>

void testcase() {
  int n; std::cin >> n;
  std::vector<int> bombs(n);
  
  for (int i = 0; i < n; ++i) {
    std::cin >> bombs[i];
  }
  
  for (int i = 0; i <= (n-3)/2; ++i) {
    int up = bombs[i];
    bombs[2*i+1] = std::min(bombs[2*i+1], up);
    bombs[2*i+2] = std::min(bombs[2*i+2], up);
  }
  
  std::sort(bombs.begin(), bombs.end());
  
  int currt = 0; // current time we're at
  while (currt < n && bombs[currt] > currt) ++currt;
  
  if (currt == n) std::cout << "yes\n";
  else std::cout << "no\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}