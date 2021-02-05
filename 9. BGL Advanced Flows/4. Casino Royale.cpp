#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long>>>>> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

void testcase() {
  int n, m, l; std::cin >> n >> m >> l;
  
  graph G(n);
  edge_adder adder(G);
  int max_cost = std::pow(2, 7);
  
  auto source = boost::add_vertex(G);
  auto target = boost::add_vertex(G);
  
  adder.add_edge(source, 0, l, 0); // upper capacity
  adder.add_edge(n-1, target, l, 0); // upper capacity
  
  for (int i = 0; i < n-1; ++i) {
    adder.add_edge(i, i+1, l, max_cost); // upper bound cost
  }
  
  for (int i = 0; i < m; ++i) {
    int x, y, q; std::cin >> x >> y >> q;
    adder.add_edge(x, y, 1, (y-x)*max_cost - q); // add a path from x to y that is faster by q
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, source, target);
  int cost = boost::find_flow_cost(G);
  std::cout << l * (n-1) * max_cost - cost << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >>t;
  while (t--) testcase();
}