#include <iostream>
#include <vector>

struct Path {
  int to;
  long val;
};

typedef std::vector<std::vector<long>> M;

M memo;
std::vector<std::vector<Path>> paths;
long x;
int k;

// return number of max points that can be achieved in moves_left moves
long dp(int i, int moves_left) {
  if (moves_left == 0) return 0;
  if (memo[i][moves_left] != -1) return memo[i][moves_left];
  
  if (paths[i].empty()) memo[i][moves_left] = dp(0, moves_left);
  else {
    long best = 0;
    for (Path p : paths[i]) {
      if ((p.val + dp(p.to, moves_left-1)) > best) {
        best = p.val + dp(p.to, moves_left-1);
      }
    }
    memo[i][moves_left] = best;
  }
  
  return memo[i][moves_left];
}

void testcase() {
  int n, m; std::cin >> n >> m >> x >> k;
  
  paths = std::vector<std::vector<Path>>(n);
  
  for (int i = 0; i < m; ++i) {
    int u, v; long p; std::cin >> u >> v >> p;
    paths[u].push_back({v, p});
  }
  
  memo = M(n, std::vector<long>(k+1, -1));
  long mp = dp(0, k);
  if (mp < x) std::cout << "Impossible\n";
  else {
    int i = k;
    while (dp(0, i) >= x) --i;
    std::cout << i+1 << '\n';
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
}