#include <iostream>
#include <vector>
#include <algorithm>

void testcase() {
  int n; std::cin >> n;
  std::vector<int> vec(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> vec[i];
  }
  
  int x; std::cin >> x;
  if (x == 0) std::sort(vec.begin(), vec.end());
  else std::sort(vec.begin(), vec.end(), std::greater<int>());
  
  for (int a : vec) {
    std::cout << a << " ";
  }
  std::cout << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}