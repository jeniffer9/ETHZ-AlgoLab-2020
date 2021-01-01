#include <iostream>
#include <set>

void testcase() {
  std::set<int> s;
  int q; std::cin >> q;
  for (int i = 0; i < q; ++i) {
    int a, b; std::cin >> a >> b;
    if (a == 1) {
      auto ix = s.find(b);
      if (ix != s.end()) {
        s.erase(ix);
      }
    } else {
      s.insert(b);
    }
  }
  if (s.empty()) {
    std::cout << "Empty\n";
  } else {
    for (int i: s) {
      std::cout << i << " ";
    }
    std::cout << "\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; ++i) {
    testcase();
  }
  return 0;
}