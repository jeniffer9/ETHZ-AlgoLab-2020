#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <numeric>

std::vector<std::vector<int>> stages;
std::vector<int> costs;
std::vector<std::vector<int>> memo;

// parent_covered = parent is covered, cover_it = cover certain ix
int algo(int ix, bool parent_covered, bool cover_it) {
  if (stages[ix].empty()) { // leaf
    if (parent_covered) return 0;
    else return costs[ix];
  }
  
  int stage = cover_it ? 0 : (parent_covered ? 1 : 2); // three stages
  
  // return if in memo
  if (memo[ix][stage] != -1) return memo[ix][stage];
  
  if (cover_it) { // cover this node
    memo[ix][stage] = costs[ix];
    
    for (int child : stages[ix]) {
      memo[ix][stage] += std::min(algo(child, true, false), algo(child, true, true));
    }
  } else if (parent_covered) { // && !cover_it
    memo[ix][stage] = 0; // don't cover
    
    for (int child : stages[ix]) {
      memo[ix][stage] += std::min(algo(child, false, true), algo(child, false, false));
    }
  } else { // parent not covered => at least one child needs to be covered
    memo[ix][stage] = 0;
    
    bool ok = false;
    int nchildren = stages[ix].size();
    std::vector<int> mins(nchildren);
    std::vector<int> maxs(nchildren);
    
    int minn = std::numeric_limits<int>::max(), minni = -1; // minn index
    for (int i = 0; i < nchildren; ++i) {
      int safe = algo(stages[ix][i], false, true); // cover child, this is also covered
      int ns = algo(stages[ix][i], false, false); // another child has to be covered
      if (safe <= ns) { // there is a strategy that is cheaper and the child is covered
        ok = true; // safe to add all the min cost values
        mins[i] = safe;
      } else {
        mins[i] = ns;
        maxs[i] = safe;
        if (!ok && safe - ns < minn) { // see if the difference btw the two strats i less than for other children
          minn = safe - ns;
          minni = i; // save index
        }
      }
    }
    
    if (ok) {
      memo[ix][stage] = std::accumulate(mins.begin(), mins.end(), 0); // add all mins together
    } else {
      memo[ix][stage] = std::accumulate(mins.begin(), mins.begin()+minni, 0);
      memo[ix][stage] += maxs[minni];
      memo[ix][stage] += std::accumulate(mins.begin()+minni+1, mins.end(), 0);
    }
  }
  
  return memo[ix][stage];
}

void testcase() {
  int n; std::cin >> n;
  
  stages = std::vector<std::vector<int>>(n);
  memo = std::vector<std::vector<int>>(n, std::vector<int>(3, -1));
  
  for (int i = 0; i < n-1; ++i) {
    int a, b; std::cin >> a >> b;
    stages[a].push_back(b);
  }
  
  costs = std::vector<int>(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> costs[i];
  }
  
  std::cout << std::min(algo(0, false, false), algo(0, false, true)) << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}