#include <iostream>
#include <vector>

void testcase() {
  int n; std::cin >> n;
  std::vector<int> v(n); // instantiate n default vector elements
  for (int i = 0; i < n; ++i) {
    std::cin >> v[i];
  }
  int d; std::cin >> d;
  v.erase(v.begin() + d); // removes element at index d
  int a, b; std::cin >> a >> b;
  v.erase(v.begin() + a, v.begin() + b + 1); // removes elements from index a to b
  
  if (v.empty()) {
    std::cout << "Empty\n";
  } else {
    for (auto i : v) {
      std::cout << i << " ";
    }
    std::cout << "\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}