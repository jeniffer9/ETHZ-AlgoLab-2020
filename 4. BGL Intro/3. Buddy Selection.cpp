#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor V;

void testcase() {
  int n, c, f; std::cin >> n >> c >> f;
  std::vector<std::vector<std::string>> characteristics(n, std::vector<std::string>(c));
  
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < c; ++j) {
      std::cin >> characteristics[i][j];
    }
    // sort for faster searching in later step
    std::sort(characteristics[i].begin(), characteristics[i].end());
  }
  
  graph G(n);
  for (int i = 0; i < n-1; ++i) {
    for (int j = i+1; j < n; ++j) {
      int m = 0; // #matches
      
      auto it1 = characteristics[i].begin();
      auto it2 = characteristics[j].begin();
      while (m <= f && it1 < characteristics[i].end() && it2 < characteristics[j].end()) {
        auto s1 = *it1;
        auto s2 = *it2;
        if (s1 == s2) {
          ++m;
          ++it1; ++it2;
        } else if (s1 > s2) ++it2;
        else ++it1;
      }
      
      // only add an edge if > f characteristics match
      if (m > f) {
        boost::add_edge(i, j, G);
      }
    }
  }
  
  std::vector<V> mate_map(n);
  boost::edmonds_maximum_cardinality_matching(G, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  
  if (matching_size == n/2) std::cout << "not optimal\n";
  else std::cout << "optimal\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
 }