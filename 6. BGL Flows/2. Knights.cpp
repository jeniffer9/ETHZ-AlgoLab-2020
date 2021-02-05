#include <iostream>

// BGL include
#include <boost/graph/adjacency_list.hpp>
// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
  graph &G;
 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, int c) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = c;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
  int m, n, k, c; std::cin >> m >> n >> k >> c;
  int nv = n*m; // number of vertices
  
  graph G(2*nv); // cause of vertex capacities
  edge_adder adder(G);

  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  for (int i = 0; i < k; ++i) {
    int x, y; std::cin >> x >> y;
    adder.add_edge(v_source, y*m+x, 1); // source to knight
  }
  
  // edge cases
  if (n == 0 || m == 0 || k == 0) {
    std::cout << 0 << "\n";
    return;
  }
  
  for (int i = 0; i < nv; ++i) {
    adder.add_edge(i, i + nv, c); // vertex capacity
  }
  
  // corner points have two ways out
  adder.add_edge(nv, v_sink, 2); // 0
  adder.add_edge(m-1+nv, v_sink, 2); // last on first row
  adder.add_edge((n-1)*m+nv, v_sink, 2); // first of last row
  adder.add_edge(nv-1+nv, v_sink, 2); //last
  
  // first and last row (w/o corners) have one way out
  for (int i = 1; i < m-1; ++i) {
    adder.add_edge(i+nv, v_sink, 1); // first row
    adder.add_edge(m*(n-1)+i+nv, v_sink, 1); // last row
  }
  
  // first and last column (w/o corners) have one way out
  for (int j = 1; j < n-1; ++j) {
    adder.add_edge(m*j+nv, v_sink, 1); // first column
    adder.add_edge(m*(j+1)-1+nv, v_sink, 1); // last column
  }
  
  // transitions between intersections
  for (int j = 0; j < n-1; ++j) {
    for (int i = 0; i < m-1; ++i) {
      adder.add_edge(m*j+i+nv, (j+1)*m+i, 1); // path down
      adder.add_edge((j+1)*m+i+nv, m*j+i, 1); // path up
      adder.add_edge(m*j+i+nv, j*m+i+1, 1); // path right
      adder.add_edge(j*m+i+1+nv, m*j+i, 1); // path left
    }
  }
  
  // connect last column with intersection below
  for (int j = 1; j < n; ++j) {
    adder.add_edge(m*j-1+nv, (j+1)*m-1, 1);
    adder.add_edge((j+1)*m-1+nv, m*j-1, 1);
  }
  
  // connect last row with intersection right
  for (int i = 0; i < m-1; ++i) {
    adder.add_edge(m*(n-1)+i+nv, (n-1)*m+i+1, 1);
    adder.add_edge((n-1)*m+i+1+nv, m*(n-1)+i, 1);
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  
  std::cout << flow << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}