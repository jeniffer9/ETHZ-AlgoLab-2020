#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>
                              > weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor Edge;

void testcase() {
  int n, m, k, x, y; std::cin >> n >> m >> k >> x >> y;
  
  // one layer for each number of rivers visited
  weighted_graph G(n * (k+1));
  weight_map weights = boost::get(boost::edge_weight, G);
  
  for (int i = 0; i < m; ++i) {
    int a, b, d, c;
    std::cin >> a >> b >> c >> d;
    
    Edge e;
    for (int r = 0; r < k; ++r) {
      // construct an edge to the next layer if it passes a river
      if (d) {
        e = boost::add_edge(a+r*n, b+(r+1)*n, G).first;
        weights[e] = c;
        e = boost::add_edge(b+r*n, a+(r+1)*n, G).first; // IMPORTANT can ascend both ways
        weights[e] = c;
      } else {
        e = boost::add_edge(a+r*n, b+r*n, G).first;
        weights[e] = c;
      }
    }
    e = boost::add_edge(a+k*n, b+k*n, G).first;
    weights[e] = c;
  }
  
  std::vector<int> dist_map(n*(k+1));
  boost::dijkstra_shortest_paths(G, x,
    boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
                        boost::get(boost::vertex_index, G))));
  
  std::cout << dist_map[k*n+y] << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}