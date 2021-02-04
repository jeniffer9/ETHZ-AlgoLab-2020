#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  edge_desc add_edge(int from, int to, long capacity, long cost) {
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
    return e;
  }
};

void testcase() {
  int c, g, b, k, a; std::cin >> c >> g >> b >> k >> a;
  
  graph G(c+1);
  edge_adder adder(G);
  
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  const int source = c;
  
  edge_desc first_edge;
  
  for (int i = 0; i < g; ++i) {
    int x, y, d, e; std::cin >> x >> y >> d >> e;
    adder.add_edge(x, y, e, d);
  }
  
  if (b == 0 || g == 0) {
    std::cout << 0 << '\n';
    return;
  }
  
  int l = 1; int h = boost::push_relabel_max_flow(G, k, a);
  first_edge = adder.add_edge(source, k, 0, 0);
  int best = 0;
  while (l <= h) {
    int mid = (h-l)/2 + l;
    
    c_map[first_edge] = mid; // update the capacity from the source to the first city
    boost::successive_shortest_path_nonnegative_weights(G, source, a);
    
    int cost = boost::find_flow_cost(G);
    int flow = mid - rc_map[first_edge];
    
    if (flow < mid && cost <= b) {
      best = flow;
      break;
    } else if (cost <= b) { // might be room for improvement
      best = mid;
      l = mid+1;
    } else if (cost > b) {
      h = mid-1;
    }
  }
  
  std::cout << best << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}