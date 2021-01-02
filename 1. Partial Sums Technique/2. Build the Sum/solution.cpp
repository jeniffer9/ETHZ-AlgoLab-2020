#include <iostream>

void testcase() {
  int n; std::cin >> n;
  int sum = 0;
  for (int i = 0; i < n; ++i) {
    int a; std::cin >> a;
    sum += a;
  }
  
  std::cout << sum << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}