#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph;
                
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

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

struct Req {
  int s;
  int t;
  int d;
  int a;
  int p;
};

int N, S;
const long MAX_TIME = 100000;

void testcase() {
  std::cin >> N >> S;
  
  const int INF = S*100;
  
  std::vector<int> cars(S);
  int flow = 0; // how many cars are there
  for (int i = 0; i < S; ++i) {
    std::cin >> cars[i];
    flow += cars[i];
  }
  
  // make sets of times used initialized with the starting and end time
  std::set<int> init_set; init_set.insert(0); init_set.insert(MAX_TIME);
  std::vector<std::set<int>> times(S, init_set); // each station has a separate set
  std::vector<std::map<int, int>> times_map(S); // each station maps the real time to the compressed time stamp
  std::vector<Req> requests(N);
  
  for (int i = 0; i < N; ++i) {
    int s, t, d, a, p; std::cin >> s >> t >> d >> a >> p;
    times[s-1].insert(d); // departure time for source station
    requests[i] = Req{s-1, t-1, d, a, p};
    times[t-1].insert(a); // arrival time for target station
  }
  
  int nn = 0;
  for (int i = 0; i < S; ++i) {
    for (auto it = times[i].begin(); it != times[i].end(); ++it) {
      int realtime = *it;
      times_map[i].insert({realtime, nn++}); // each realtime gets a different index for each station 
    }
  }
  
  // nn = time points
  graph G(nn+2);
  int source = nn;
  int sink = nn+1;
  edge_adder adder(G);
  
  for (auto r : requests) {
    // add from time point of source station one capacity edge with the compressed cost minus the profit
    adder.add_edge(times_map[r.s][r.d], times_map[r.t][r.a], 1, 100*(r.a-r.d)-r.p);
  }
  
  nn = 0;
  for (int i = 0; i < S; ++i) { // for all station
    adder.add_edge(source, nn, cars[i], 0); // add an edge with capacity of initial cars
    int ix = 0; int last = 0; // keep the index and the previous time
    for (auto& t : times[i]) {
      if (ix == 0) { // ignore the starting points
        nn++; ix++;
      } else {
        // add an INF capacity edge from previous time stamp (stay parked) with compressed cost
        adder.add_edge(nn-1, nn, INF, (t-last)*100); nn+=1;
      }
      last = t;
    }
    // add an INF capacity edge from end time to the sink for free
    adder.add_edge(nn-1, sink, INF, 0);
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  std::cout << MAX_TIME*100*flow-boost::find_flow_cost(G) << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}