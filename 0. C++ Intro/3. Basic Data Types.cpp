#include <iostream>
#include <string>
#include <iomanip> // for manipulation of output precisions

void testcase() {
  int i; long l; std::string s; double d;
  std::cin >> i >> l >> s >> d;
  std::cout << i << " " << l << " " << s << " " << d << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  // std::fixed applies precision to floating points
  std::cout << std::setprecision(2) << std::fixed;
  int t; std::cin >> t;
  while (t--) testcase();
}