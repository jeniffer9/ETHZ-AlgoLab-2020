#include <iostream>

#include <boost/graph/adjacency_list.hpp>

#include <boost/graph/push_relabel_max_flow.hpp>

#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph;

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = capacity; // undirected!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = cost;   // undirected!
  }
};

void dijkstra_path(const graph &G, int s, std::vector<int>& dist_map) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> pred_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));
}


void testcase() {
  int n, m, s, f; std::cin >> n >> m >> s >> f;
  
  graph G(n);
  edge_adder adder(G);
  
  auto w_map = boost::get(boost::edge_weight, G);
  auto c_map = boost::get(boost::edge_capacity, G);
  
  for (int i = 0; i < m; ++i) {
    int a, b, c, d; std::cin >> a >> b >> c >>d;
    if (a != b) {// dont add cycles
      adder.add_edge(a, b, c, d);
    }
  }
  
  std::vector<int> dist_map_s(n); dijkstra_path(G, s, dist_map_s);
  std::vector<int> dist_map_f(n); dijkstra_path(G, f, dist_map_f);
  
  int shortest = dist_map_s[f];
  for (auto edge_it = boost::edges(G).first; edge_it != boost::edges(G).second; ++edge_it) {
    const edge_desc edge = *edge_it;
    const auto a = boost::source(edge, G), b = boost::target(edge, G);
    if (dist_map_s[a] + w_map[edge] + dist_map_f[b] > shortest)
      // if this edge does not belong to fastest route, remove flow possibility
      c_map[edge] = 0;
  }
  
  std::cout << boost::push_relabel_max_flow(G, s, f) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}