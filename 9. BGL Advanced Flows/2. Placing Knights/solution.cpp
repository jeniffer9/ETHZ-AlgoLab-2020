#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

typedef std::vector<int> V;
typedef std::vector<V> M;

void testcase() {
  int n; std::cin >> n;
  M fields(n, V(n));
  
  int possible_knights = 0;
  
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cin >> fields[i][j];
      if (fields[i][j]) ++possible_knights;
    }
  }
  
  graph G(n*n);
  for (int i = 0; i < n-2; ++i) {
    for (int j = 0; j < n-1; ++j) {
      if (fields[i][j] && fields[i+2][j+1]) boost::add_edge(i*n+j, (i+2)*n+j+1, G);
    }
  }
  for (int i = 0; i < n-2; ++i) {
    for (int j = 1; j < n; ++j) {
      if (fields[i][j] && fields[i+2][j-1]) boost::add_edge(i*n+j, (i+2)*n+j-1, G);
    }
  }
  for (int i = 0; i < n-1; ++i) {
    for (int j = 0; j < n-2; ++j) {
      if (fields[i][j] && fields[i+1][j+2]) boost::add_edge(i*n+j, (i+1)*n+j+2, G);
    }
  }
  for (int i = 0; i < n-1; ++i) {
    for (int j = 2; j < n; ++j) {
      if (fields[i][j] && fields[i+1][j-2]) boost::add_edge(i*n+j, (i+1)*n+j-2, G);
    }
  }
  
  std::vector<vertex_desc> mate_map(n*n);
  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  
  std::cout << possible_knights-matching_size << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}