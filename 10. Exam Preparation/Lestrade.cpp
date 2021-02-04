#include <iostream>
#include <vector>
#include <cmath>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Delaunay;

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct GM {
  int u;
  int v;
  int w;
};

void testcase() {
  int z, u, v, w; std::cin >> z >> u >> v >> w;
  int a, g; std::cin >> a >> g;
  
  typedef std::pair<P, int> PR; // info pair for vertices
  
  std::vector<PR> pts(g);
  std::vector<GM> gang_members(g);
  for (int i = 0; i < g; ++i) {
    int x, y, ui, vi, wi; std::cin >> x >> y >> ui >> vi >> wi;
    pts[i] = PR{P(x,y), i};
    gang_members[i] = GM{ui, vi, wi};
  }
  
  Delaunay t;
  t.insert(pts.begin(), pts.end());
  
  std::vector<int> agents(a);
  std::vector<int> agent_member(g, -1);
  for (int i = 0; i < a; ++i) {
    int x, y, zi; std::cin >> x >> y >> zi;
    agents[i] = zi;
    int gi = t.nearest_vertex(P(x, y))->info(); // nearest gang member index 
    if (agent_member[gi] == -1 || zi < agents[agent_member[gi]]) // if no agent assigned or cheaper agent found
      agent_member[gi] = i; // agent index of agent with minimum salary
  }
  
  Program lp (CGAL::LARGER, true, 0, true, 24); // all hi constraints
  
  lp.set_b(0, u); // all hi * uj adder must be at least u
  lp.set_b(1, v); // all hi * vj adder must be at least v
  lp.set_b(2, w); // all hi * wj adder must be at least w

  int ai = 0;
  for (int i = 0; i < g; ++i) {
    int a = agent_member[i];
    if (a < 0) continue; // no agent assigned
    int az = agents[a]; // hourly wage
    
    GM gm = gang_members[i];
    
    lp.set_a(ai, 0, gm.u); // adding these up must be at least u
    lp.set_a(ai, 1, gm.v); // adding these up must be at least v
    lp.set_a(ai, 2, gm.w); // adding these up must be at least w
    
    lp.set_c(ai, az); // price of agent i for ai hours
    ++ai;
  }

  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.is_infeasible() || s.objective_value() > z) std::cout << "H\n";
  else std::cout << "L\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}