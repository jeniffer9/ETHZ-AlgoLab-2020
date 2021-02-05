#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <algorithm>

typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>
                              > graph;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor E;

void testcase() {
  int n, m; std::cin >> n >> m;
  
  graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  
  for (int i = 0; i < m; ++i) {
    int u, v, w; std::cin >> u >> v >> w;
    E e = boost::add_edge(u, v, G).first;
    weights[e] = w;
  }
  
  std::vector<E> mst; // vector to store MST edges (not a property map)
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  
  int sum = 0;
  for (std::vector<E>::iterator it = mst.begin(); it != mst.end(); ++it) {
    sum += weights[*it];
  }
  
  std::vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(G, 0,
    boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
                        boost::get(boost::vertex_index, G))));
                        
  auto di = std::max_element(dist_map.begin(), dist_map.end());
  
  std::cout << sum << " " << *di << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}