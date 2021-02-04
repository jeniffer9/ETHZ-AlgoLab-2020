#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

// Beverage
struct B {
  int v;
  int c;
};

// memo entry
struct E {
  int c;
  int np;
  bool in;
};

const int INF = std::numeric_limits<int>::max();
std::vector<B> ch;

void testcase() {
  int n, k; std::cin >> n >> k;
  
  std::vector<B> beverages;
  beverages.reserve(n);
  
  for (int i = 0; i < n; ++i) {
    int c, v; std::cin >> c >> v;
    beverages.emplace_back(B{v, c});
  }

  // how cheap can I achieve k liters just using drink 1 to i
  // how many drinks are used and is drink i included 
  std::vector<std::vector<E>> memo(n+1);
  for (int i = 0; i < n+1; ++i) {
    // initialize vector from 1 to k for each drink
    memo[i] = std::vector<E>(k+1, E{0, 0, false});
  }
  
  for (int i = 0; i < k; ++i) {
    // initialize drink 0 (buffer) with infinite cost for any liter > 0
    memo[0][i+1] = E{INF, 0, false};
  }
  
  for (int i = 1; i <= n; ++i) { // for each drink
    for (int j = 1; j <= k; ++j) { // up to k liters (we don't care about more)
      auto prev = memo[i-1][j]; // how much for up to previous drink
      auto th = memo[i][std::max(j-beverages[i-1].v, 0)]; // how much when taking this drink
      int nc = beverages[i-1].c + th.c; // new cost (i-1 cause of 0 index)
      int nnp = th.np + !th.in; // add 1 if previously ith drink was not included
      if (prev.c < nc || (prev.c == nc && prev.np >= nnp)) { // if without i, it's cheaper or same price and more people
        memo[i][j] = E{prev.c, prev.np, false}; // don't include drink i
      } else {
        memo[i][j] = E{nc, nnp, true}; // else include drink i
      }
    }
  }
  
  std::cout << memo[n][k].c << ' ' << memo[n][k].np << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
}