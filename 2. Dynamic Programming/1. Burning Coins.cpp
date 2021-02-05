#include <iostream>
#include <vector>
#include <cmath>

typedef std::vector<int> V;
typedef std::vector<V> M;

M memo;
V coins;

int best(int i, int j, int s) { // instead of keeping the sum of each individual player, keep track of the sum on the table
  if (memo[i][j] != -1) return memo[i][j];
  // std::min here because we're in control (calculating the BEST for the current player)
  // so we chose the side which has the minimal best for the other
  // same as (std::max(s - best(i, j-1, s-coins[j]), s - best(i+1, j, s - coins[i])))
  memo[i][j] = s - std::min(best(i+1, j, s - coins[i]), best(i, j-1, s-coins[j]));
  return memo[i][j];
}

void testcase() {
  int n; std::cin >> n;
  int sum = 0; // sum of all coins on the table
  coins = V(n);
  memo = M(n, V(n, -1));
  
  for (int i = 0; i < n; ++i) {
    std::cin >> coins[i];
    sum += coins[i];
    memo[i][i] = coins[i];
  }
  
  std::cout << best(0, n-1, sum) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}