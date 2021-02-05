#include <iostream>
#include <string>
#include <utility> // for std::swap function
#include <algorithm> // for std::reverse function

void testcase() {
  std::string a, b; std::cin >> a >> b;
  std::cout << a.length() << " " << b.length() << "\n";
  std::cout << a + b << "\n";
  std::reverse(a.begin(), a.end());
  std::reverse(b.begin(), b.end());
  std::swap(a[0], b[0]);
  std::cout << a << " " << b << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}