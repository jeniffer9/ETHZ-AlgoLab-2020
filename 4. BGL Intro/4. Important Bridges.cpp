#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

namespace boost
{
  struct edge_component_t
  {
      enum
      {
          num = 555
      };
      typedef edge_property_tag kind;
  } edge_component;
}

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_component_t, int>
                              > graph;
typedef boost::property_map<graph, boost::edge_component_t>::type ComponentMap;
typedef boost::graph_traits<graph>::edge_descriptor E;
typedef boost::graph_traits<graph>::edge_iterator EIT;

void testcase() {
  int n, m; std::cin >> n >> m;
  graph G(n);
  
  if (n == 0 || m == 0) {
    std::cout << 0 << '\n';
    return;
  }
  
  for (int i = 0; i < m; ++i) {
    int e1, e2; std::cin >> e1 >> e2;
    boost::add_edge(e1, e2, G);
  }
  
  ComponentMap component = boost::get(boost::edge_component, G);
  int nbc = biconnected_components(G, component);
  
  std::vector<std::vector<E>> comp_edges(nbc);
  EIT ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei) {
    comp_edges[component[*ei]].push_back(*ei);
  }
  
  int sol = 0;
  std::vector<std::pair<int, int>> vs;
  for (int i = 0; i < nbc; ++i) {
    if (comp_edges[i].size() == 1) {
      ++sol;
      E e = comp_edges[i][0];
      int a = boost::source(e, G);
      int b = boost::target(e, G);
      if (a > b) std::swap(a, b);
      vs.push_back(std::make_pair(a, b));
    }
  }
  
  std::sort(vs.begin(), vs.end(),
            [](const std::pair<int, int>& e1, const std::pair<int, int>& e2) -> bool {
                return e1.first < e2.first ||
                (e1.first == e2.first && e1.second < e2.second);
            });
  
  std::cout << sol << '\n';
  for (auto pr : vs) {
    std::cout << pr.first << " " << pr.second << '\n';
  }
}


int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}