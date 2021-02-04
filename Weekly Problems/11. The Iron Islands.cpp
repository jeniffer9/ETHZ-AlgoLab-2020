#include <iostream>
#include <vector>
#include <map>

void testcase() {
  int n, k, w; std::cin >> n >> k >> w;

  std::vector<int> costs(n);
  
  for (int i = 0; i < n; ++i) {
    std::cin >> costs[i];
  }
  
  std::vector<std::vector<int>> waterways(w);
  for (int i = 0; i < w; ++i) {
    int l; std::cin >> l;
    waterways[i] = std::vector<int>(l);
    for (int j = 0; j < l; ++j) {
      std::cin >> waterways[i][j];
    }
  }
  
  std::map<int, int> sub_sums; sub_sums[0] = 0; // map subsum to number of islands

  int opt = 0;
  for (int i = 0; i < w; ++i) { // go through all waterways
    std::vector<int> single_sub_sum;
    int ws = (int)waterways[i].size();
    int j1 = 1; int sum = 0;
    while (j1 < ws && sum-costs[0] < k) {
      int c = costs[waterways[i][j1]];
      sum += c; // subsum from island 1 to island j1
      if (sum-costs[0] < k) {
        single_sub_sum.push_back(sum);
      }
      j1++;
    }

    int i1 = 0; j1 = 0; sum = costs[0]; // set j1 back to where it ended
    while (j1 < ws) {
      if (sum == k) {
        opt = std::max(j1-i1+1, opt);
        sum -= costs[waterways[i][i1]];
        ++i1; ++j1;
        if (j1 < ws) sum += costs[waterways[i][j1]];
      } else if (sum < k) {
        ++j1;
        if (j1 < ws) sum += costs[waterways[i][j1]];
      } else { // sum > k
        sum -= costs[waterways[i][i1]];
        ++i1;
        if (i1 > j1) {
          j1 = i1;
          if (j1 < ws) sum = costs[waterways[i][j1]];
        }
      }
    }
    
    for (int jj = 0; jj < (int)single_sub_sum.size(); ++jj) {
      auto it = sub_sums.find(k - costs[0] - single_sub_sum[jj]);
      
      if (it != sub_sums.end()) {
        opt = std::max(opt, jj + 2 + it->second);
      }
    }
    
    for (int jj = 0; jj < (int)single_sub_sum.size(); ++jj) {
      int men = single_sub_sum[jj];
      // keep subsum with most number of islands
      sub_sums[men] = std::max(sub_sums[men], jj+1);
    }
  }

  std::cout << opt << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}