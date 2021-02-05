#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS,
                              boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef std::vector<std::vector<char>> M;

void testcase() {
  int w, h; std::cin >> w >> h;
  
  M grid(h, std::vector<char>(w));
  int points = 0;
  graph G(w*h);
  
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      std::cin >> grid[i][j];
      if (grid[i][j] == '.') ++points;
    }
  }
  
  if (points % 2 == 1) {
    std::cout << "no\n";
    return;
  }
  
  for (int i = 0; i < h-1; ++i) {
    for (int j = 0; j < w-1; ++j) {
      if (grid[i][j] == '.') {
        if (grid[i][j+1] == '.') boost::add_edge(i*w+j, i*w+j+1, G);
        if (grid[i+1][j] == '.') boost::add_edge(i*w+j, (i+1)*w+j, G);
      }
    }
  }
  
  // last row
  for (int j = 0; j < w-1; ++j) {
    if (grid[h-1][j] == '.') {
      if (grid[h-1][j+1] == '.')
        boost::add_edge((h-1)*w+j, (h-1)*w+j+1, G);
    }
  }
  
  // last col
  for (int i = 0; i < h-1; ++i) {
    if (grid[i][w-1] == '.') {
      if (grid[i+1][w-1] == '.')
        boost::add_edge((i+1)*w-1, (i+2)*w-1, G);
    }
  }
  
  std::vector<vertex_desc> mate_map(w*h);
  boost::edmonds_maximum_cardinality_matching(G, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  if (matching_size == points/2) std::cout << "yes\n";
  else std::cout << "no\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
 }