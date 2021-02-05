#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> V;
typedef std::vector<V> M;

M memo; // for m attackers what is the highest max defense value
V pairs;
int n;
int m;
int k;

int dp(int attackers, int defender) {
  if (attackers == m) return 0;
  if (defender == n) return -1;
  if (memo[attackers][defender] == -2) {
    int take = -1; // take this interval
    if (pairs[defender] != -1) {
      take = dp(attackers+1, pairs[defender]+1);
      
      if (take != -1) {
        take += pairs[defender]-defender+1;
      }
    }
    
    int skip = dp(attackers, defender+1);
    memo[attackers][defender] = std::max(skip, take);
  }
  return memo[attackers][defender];
}

void testcase() {
  std::cin >> n >> m >> k;
  memo = M(m, V(n, -2)); // -2 = not implemented yet
  V d_values(n);
  
  for (int i = 0; i < n; ++i) {
    std::cin >> d_values[i];
  }
  
  pairs = V(n, -1); // store the pairs that actually have a k value
  
  int left = 0, right = 0, sum = d_values[0];
  while (right < n) {
    if (sum == k) {
      pairs[left] = right;
      sum -= d_values[left++]; 
      if (left > right) {
        right = left;
        if (right < n) sum += d_values[right];
      }
    } else if (sum < k) {
      ++right;
      if (right < n) sum += d_values[right];
    } else { // sum > k
      sum -= d_values[left++];
      if (left > right) {
        right = left;
        if (right < n) sum = d_values[right];
      }
    }
  }
  
  int sol = dp(0, 0);
  sol < 0 ? std::cout << "fail\n" : std::cout << sol << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}