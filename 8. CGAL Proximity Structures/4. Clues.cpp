#include <iostream>
#include <vector>
#include <queue>
#include <string>
 
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
 
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

typedef CGAL::Triangulation_vertex_base_with_info_2<std::pair<int, int>, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Vertex_handle V;
  
void initialize(Triangulation &t) {
  for(auto f = t.finite_vertices_begin(); f != t.finite_vertices_end(); ++f) {
    f->info() = std::make_pair(0, 0); // frequency, network number
  }
}
 
bool is_ok_network(Triangulation &t, K::FT sq_r) {
  int nn = 0; // network number
  for(auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v) {
    if (!v->info().first) { // unvisited
      // BFS
      std::queue<V> q;
      v->info() = std::make_pair(1, nn);
      q.push(v);
      
      while (!q.empty()) {
        V cv = q.front(); q.pop();
        std::vector<V> neighbor_set;
        
        Triangulation::Vertex_circulator c = t.incident_vertices(cv);
        do {
          if (!t.is_infinite(c) && CGAL::squared_distance(cv->point(), c->point()) <= sq_r){
            if (c->info().first == cv->info().first) return false; // same frequency
            else if (!c->info().first) {
              c->info() = std::make_pair(cv->info().first ^ 3, nn);
              q.push(c);
            }
            neighbor_set.push_back(c);
          }
        } while (++c != t.incident_vertices(cv));
        if (neighbor_set.size() > 1) { // check unconnected triangles
          for(auto n1 = neighbor_set.begin(); n1 != neighbor_set.end()-1; n1++) {
            for(auto n2 = n1+1; n2 != neighbor_set.end(); n2++) {
              if (CGAL::squared_distance((*n1)->point(), (*n2)->point()) <= sq_r) {
                return false;
              }
            }
          }
        }
      }
      ++nn; // increase network number once queue is empty
    }
  }
 
  return true;
}
 
void testcase() {
  int n, m; long r; std::cin >> n >> m >> r;
  K::FT sq_r(r*r);
  std::vector<P> stations(n), clues(2*m);
  
  for (int i = 0; i < n; ++i) std::cin >> stations[i];
  for (int i = 0; i < m; ++i) std::cin >> clues[i] >> clues[i+m];
  
  Triangulation t;
  t.insert(stations.begin(), stations.end());
  initialize(t);
  
  // Check for network with interferences
  if (!is_ok_network(t, sq_r)) {
    std::cout << std::string(m, 'n') << "\n";
    return;
  }
  
  for (int i = 0; i < m; ++i) {
    P a = clues[i], b= clues[i+m];
    if (CGAL::squared_distance(a, b) <= sq_r) {
      std::cout << 'y';
    } else {
      V va = t.nearest_vertex(clues[i]);
      V vb = t.nearest_vertex(clues[i+m]);
      if (va->info().second == vb->info().second
          && CGAL::squared_distance(a, va->point()) <= sq_r
          && CGAL::squared_distance(b, vb->point()) <= sq_r) std::cout << 'y';
      else std::cout << 'n';
    }
  }
  
  std::cout << '\n';
}
 
int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) {
    testcase();
  }
}