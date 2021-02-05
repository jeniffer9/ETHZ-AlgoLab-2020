#include <iostream>
#include <vector>
#include <algorithm>

int n, k, m;
std::vector<int> sovereigns;
std::vector<std::vector<int>> best_k; // memo how much k gets for range i-j when it's k's turn
std::vector<std::vector<int>> best_others; // memo how much k gets for range i-j when it's not k's turn

// if I am player !k =>
//    goal is to minimize best_of_k
// if I am player k =>
//    goal is to maximize(best_of_k)
int best(int i, int j, int r) { // i=index left, j=index right, r=round
  if (r % m == k) {
    if (best_k[i][j] == -1) {
      best_k[i][j] =  std::max(sovereigns[i] + best(i+1, j, r+1), sovereigns[j] + best(i, j-1, r+1));
    }
    return best_k[i][j];
  }

  // if not k's turn
  if (best_others[i][j] == -1) {
    best_others[i][j] = std::min(best(i+1, j, r+1), best(i, j-1, r+1));
  }
  return best_others[i][j];
}

void testcase() {
  std::cin >> n >> m >> k;
  sovereigns = std::vector<int>(n);
  best_others = std::vector<std::vector<int>>(n, std::vector<int>(n, -1));
  best_k = std::vector<std::vector<int>>(n, std::vector<int>(n, -1));
  for (int i = 0; i < n; ++i) {
    std::cin >> sovereigns[i];
    best_others[i][i] = 0; // crucial! if we come to the end, then k gets 0
    best_k[i][i] = sovereigns[i];
  }
  
  std::cout << best(0, n-1, 0) << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
}