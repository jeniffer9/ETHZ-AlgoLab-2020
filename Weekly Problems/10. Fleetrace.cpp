#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; 
                
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;

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

const int MAXP = 50;

void testcase() {
  int b, s, p; std::cin >> b >> s >> p;

  graph G(b+s+2);
  edge_adder adder(G);
  
  //const auto source1 = boost::add_vertex(G); // to regulate the flow
  const auto source = b+s;
  const auto sink = source+1;
  
  for (int i = 0; i < s; ++i) {
    adder.add_edge(source, i, 1, 0); // connect each sailor to the source
    adder.add_edge(i, sink, 1, MAXP); // let him have the most expensive way directly to the sink (not taking part in the race)
  }
  
  for (int i = 0; i < b; ++i) {
    adder.add_edge(i+s, sink, 1, 0); // connect each boat to the sink for free
  }
  
  for (int i = 0; i < p; ++i) {
    int bi, si, ci; std::cin >> bi >> si >> ci;
    adder.add_edge(si, bi+s, 1, MAXP-ci); // connect si with bi and a shifted cost
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  int cost = boost::find_flow_cost(G);
  std::cout << s*MAXP-cost << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}