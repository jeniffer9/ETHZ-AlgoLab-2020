#include <iostream>
#include <vector>
#include <algorithm>

struct Seg {
  int a;
  int b;
  
  bool operator <(const Seg& other) {
    return b < other.b || (b==other.b && a > other.a);
  }
};

std::pair<int, int> best(std::vector<Seg>& segs, int start, int a, int b) {
  int result = 1;
  int un = (int)segs.size();
  int last = start;
  if (a > b) {
    for (int i = start; i < un; ++i) {
      Seg s = segs[i];
      if (s.a > b && s.b < a) { // b <= m since ordered
        b = s.b;
        ++result;
        last = i;
      }
    }
    return std::make_pair(result, last);
  }
  for (int i = start; i < un; ++i) {
    Seg s = segs[i];
    if (s.a > b) { // b <= m since ordered
      b = s.b;
      ++result;
      last = i;
    }
  }
  return std::make_pair(result, last);
}


void testcase() {
  int n, m; std::cin >> n >> m;
  
  std::vector<Seg> negative_starts = {};
  std::vector<Seg> positive_starts = {};
  
  for (int i = 0; i < n; ++i) {
    int a, b; std::cin >> a >> b;
    
    if (a <= b) {
      positive_starts.push_back({a, b});
    } else {
      negative_starts.push_back({a, b});
    }
  }
  
  if (n == 1 || m == 1 || positive_starts.empty()) {
    std::cout << 1 <<'\n';
    return;
  }
  
  std::sort(positive_starts.begin(), positive_starts.end());
  std::sort(negative_starts.begin(), negative_starts.end());
  
  // only positive starts segments
  std::pair<int, int> pos_sol = best(positive_starts, 1, positive_starts[0].a, positive_starts[0].b);
  // if there are no negative start segments, this is the maximum
  if (negative_starts.empty()) {
    std::cout << pos_sol.first << '\n';
    return;
  }
  
  Seg last_pos = positive_starts[pos_sol.second]; // last segment in positive solution
  
  int nns = (int)negative_starts.size();
  bool opt = false;
  int io = 0;
  // check if there exists an optimal solution that includes the already computed solution
  while (!opt && io < nns && negative_starts[io].b < positive_starts[0].a) {
    if (negative_starts[io].a > last_pos.b) opt=true;
    ++io;
  }

  int bes = pos_sol.first;
  int lb = 0;
  for (int i = 0; !opt && i < nns; ++i) {
    Seg ns = negative_starts[i];
    if (ns.b > lb && ns.a > last_pos.b) { // only check if it's a different finish point from previous and it could be a possible improvement from the positive solution
      // start with second, cause 0 would only be included in very optimal solution (that we tested before)
      if (best(positive_starts, 1, ns.a, ns.b).first > bes) opt = true;
      lb = ns.b; // update last tried finish point
    }
  }
  
  if (opt) std::cout << pos_sol.first+1 << '\n';
  else std::cout << pos_sol.first << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}