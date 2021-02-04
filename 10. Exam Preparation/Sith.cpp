#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;


void testcase() {
  int n, r; std::cin >> n >> r;
  double sq_r = (double)r * (double)r;
  
  std::vector<std::pair<P,Index>> planets(n);
  for (int i = 0; i < n; ++i) {
    int x, y; std::cin >> x >> y;
    planets[i] = {P(x,y),i};
  }
  
  int sol = 0;
  int kl = 1; int kh = n/2;
  while (kl <= kh) { // binary search
    int k = kl + (kh-kl)/2; // midpoint
    
    Delaunay t;
    t.insert(planets.begin()+k, planets.end()); // only consider the planets that can be visited before colliding with chancellor
    
    std::vector<int> component_map(n-k, 1);
    boost::disjoint_sets_with_storage<> uf(n-k);
    int me = 1; // size of largest connected component
    
    // iterate through the edges
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
      Index i1 = e->first->vertex((e->second+1)%3)->info()-k; // reset the index
      Index i2 = e->first->vertex((e->second+2)%3)->info()-k;
      Index c1 = uf.find_set(i1);
      Index c2 = uf.find_set(i2);
      if (t.segment(*e).squared_length() <= sq_r && c1 != c2) {
        int n1 = component_map[c1]; int n2 = component_map[c2];
        uf.link(c1, c2);
        int nn = n1 + n2;
        component_map[c1] = component_map[c2] = nn;
        me = std::max(me, nn);
      }
    }
    
    if (me == k) { // if equal to k, it will only get worse if we continue
      sol = k;
      break;
    } else if (k < me) {
      kh = std::min(kh, me);
      kl = k+1;
      sol = std::max(sol, k);
    } else if (k > me) {
      kl = std::max(kl, me);
      kh = k-1;
      sol = std::max(sol, me);
    }
  }
  
  std::cout << sol << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}