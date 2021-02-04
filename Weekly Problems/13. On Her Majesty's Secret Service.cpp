#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      graph;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> ugraph;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<ugraph>::vertex_descriptor vertex_desc;

std::vector<int> dijkstra_dist(const graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);
  
  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));
  return dist_map;
}

const int INF = std::numeric_limits<int>::max();

void testcase() {
  int n, m, a, s, c, d; std::cin >> n >> m >> a >> s >> c >> d;
  
  graph G(n);
  
  weight_map weights = boost::get(boost::edge_weight, G);
  
  edge_desc e;
  // slopes & ski lifts
  for (int i = 0; i < m; ++i) {
    // S = slope, L = lift
    char w; int x, y, z; std::cin >> w >> x >> y >> z;
    e = boost::add_edge(x, y, G).first; weights[e] = z;
    if (w == 'L') {
      e = boost::add_edge(y, x, G).first; weights[e] = z;
    }
  }
  
  std::vector<std::vector<int>> dist_maps(a);
  for (int i = 0; i < a; ++i) {
    int p; std::cin>>p;
    dist_maps[i] = dijkstra_dist(G, p);
  }
  
  std::vector<std::vector<int>> T(a, std::vector<int>(s)); // time for each agent to shelter
  for (int i = 0; i < s; ++i) {
    int p; std::cin >> p;
    for (int j = 0; j < a; ++j) {
      T[j][i] = dist_maps[j][p];
    }
  }
  
  if (a == 1) {
    int sol = 1000000000;
    for (int i = 0; i < s; ++i) {
      sol = std::min(T[0][i], sol);
    }
    std::cout << sol+d << '\n';
    return;
  }
  int sol = 0;
  int l = 1, h = INF;
  // binary search for smallest d
  while (l <= h) {
    int t = (h-l)/2 + l;
    ugraph uG(a+c*s);
    for (int i = 0; i < a; ++i) { // for each agent
      for (int j = 0; j < s; ++j) { // for each station
        for (int ci = 0; ci < c; ++ci) {
          if (T[i][j] == INF) continue; // ignore unreachable shelters
          if (T[i][j] <= t-(ci+1)*d) { // keep the edges that ar less or equal to t-d
            boost::add_edge(i, ci*s+a+j, uG);
          }
        }
      }
    }
    
    std::vector<vertex_desc> mate_map(a+c*s);
    boost::edmonds_maximum_cardinality_matching(uG,
      boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, uG)));
    int matching_size = boost::matching_size(uG,
      boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, uG)));

    if (matching_size < a) {
      l = t+1;
    } else {
      sol = t;
      h = t-1;
    }
  }

  std::cout << sol << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >>t;
  while (t--) testcase();
}