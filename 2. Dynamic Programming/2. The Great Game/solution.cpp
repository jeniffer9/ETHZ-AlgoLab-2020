#include <iostream>
#include <vector>

std::vector<std::vector<int>> transitions;
std::vector<int> strategy_best;
std::vector<int> strategy_worst;
int nn;

int strategy(bool best, int i) { // i == index we're currently at
  if (best) {
    if (strategy_best[i] == -1) {
      int min = nn;
      for (int t : transitions[i]) {
        if (strategy(!best, t) < min) { // calculate the minimal opponent's strategy
          min = strategy(!best, t);
        }
      }
      strategy_best[i] = 1 + min;
    }
    return strategy_best[i];
  } else {
    if (strategy_worst[i] == -1) {
      int max = 0;
      for (int t : transitions[i]) {
        if (strategy(!best, t) > max) { // calculate the longest opponent's strategy
          max = strategy(!best, t);
        }
      }
      strategy_worst[i] = 1 + max;
    }
    return strategy_worst[i];
  }
}

void testcase() {
  int n, m; std::cin >> n >> m;
  nn = n;
  int r, b; std::cin >> r >> b;
  --r; --b;
  
  std::vector<std::vector<int>> ts(n-1);
  
  for (int i = 0; i < m; ++i) {
    int u, v; std::cin >> u >> v;
    ts[u-1].push_back(v-1);
  }

  transitions = ts;

  strategy_best = std::vector<int>(n, -1);
  strategy_worst = std::vector<int>(n, -1);
  strategy_best[n-1] = 0;
  strategy_worst[n-1] = 0;
  
  int result;
  int red = strategy(true, r), black = strategy(true, b);
  
  if (red < black) {
    result = 0;
  } else if (black < red) {
    result = 1;
  } else {
    result = red % 2 == 0 ? 1 : 0;
  }
  
  std::cout << result << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}