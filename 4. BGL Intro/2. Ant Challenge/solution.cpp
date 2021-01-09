#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>
                              > graph;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor E;

void testcase() {
  int n, e, s, a, b; std::cin >> n >> e >> s >> a >> b;
  std::vector<graph> graphs(s, graph(n)); // graph for each species
  std::vector<weight_map> weight_maps(s); // weight map for each species
  
  for (int i = 0; i < s; ++i) {
    weight_maps[i] = boost::get(boost::edge_weight, graphs[i]);
  }
  
  for (int i = 0; i < e; ++i) {
    int t1, t2; std::cin >> t1 >> t2;
    for (int j = 0; j < s; ++j) {
      E edge = boost::add_edge(t1, t2, graphs[j]).first;
      std::cin >> weight_maps[j][edge];
    }
  }
  
  // compute mst for each species (=private network)
  std::vector<std::vector<E>> msts(s);
  for (int i = 0; i < s; ++i) {
    int h; std::cin >> h; // hive for each species.. don't need this
    boost::kruskal_minimum_spanning_tree(graphs[i], std::back_inserter(msts[i]));
  }
  
  // build graph that combines all private networks
  graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  for (int i = 0; i < s; ++i) {
    for (std::vector<E>::iterator it = msts[i].begin(); it != msts[i].end(); ++it) {
      E ee = boost::add_edge(boost::source(*it, graphs[i]), boost::target(*it, graphs[i]), G).first;
      weights[ee] = weight_maps[i][*it];
    }
  }
  
  // finally run dijkstra to find the shortest path from a to b
  // (yes dijkstra also works with undirected graph and parallel edges)
  std::vector<int> dist_map(n);
  
  boost::dijkstra_shortest_paths(G, a,
    boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
      boost::get(boost::vertex_index, G))));
      
  std::cout << dist_map[b] << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
}