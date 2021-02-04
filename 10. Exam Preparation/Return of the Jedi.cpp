#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include <boost/pending/disjoint_sets.hpp>

struct Edge {
  int u;
  int v;
  int c;
};

bool operator < (const Edge& e1, const Edge& e2) {
  return (e1.c < e2.c);
}

int n;
std::vector<int> leilas;
std::vector<Edge> channels;

int compute_mst(bool leila, int ignore) {
  boost::disjoint_sets_with_storage<> uf(n);
  int n_components = n;
  int cost = 0;
  for (int i = 0; i < (int)channels.size(); ++i) {
    // determine components of endpoints
    if (i == ignore) continue;
    Edge e = channels[i];
    int c1 = uf.find_set(e.u);
    int c2 = uf.find_set(e.v);
    if (c1 != c2) {
      if (leila) leilas.push_back(i); // save edge in leila's mst
      cost += e.c;
      // this edge connects two different components => part of the emst
      uf.link(c1, c2);
      if (--n_components == 1) break;
    }
  }
  return cost;
}

void testcase() {
  int tatooine; std::cin >> n >> tatooine;
  
  channels.clear();
  for (int j = 1; j < n; ++j) {
    for (int k = 0; k < n-j; ++k) {
      int i; std::cin >> i;
      channels.push_back(Edge{j-1, j+k, i});
    }
  }
  
  std::sort(channels.begin(), channels.end());
  leilas.clear();
  compute_mst(true, -1); // compute leila's mst
  int min_cost = leilas.size()*std::pow(2, 20);
  for (auto ei : leilas) {
    min_cost = std::min(compute_mst(false, ei), min_cost);
  }
  std::cout << min_cost << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}