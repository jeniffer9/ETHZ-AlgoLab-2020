#include <iostream>
#include <vector>

void testcase() {
  int n; std::cin >> n;
  std::vector<int> bits(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> bits[i];
  }
  
  int sum =0, even = 0, odd = 0;
  for (int i = 0; i < n; ++i) {
    sum += bits[i];
    if (sum % 2 == 0) {
      ++even;
    } else {
      ++odd;
    }
  }
  
  // add any 2 even sums together and you get a new even sum [nCk C(even, 2)]
  // add any 2 odd sums together and you get an even sum [nCk C(odd, 2)]
  // don't forget to add the initial even sums
  int result = even + even * (even-1) / 2 + odd * (odd-1) / 2;
  std::cout << result << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
}