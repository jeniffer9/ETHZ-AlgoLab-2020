#include <iostream>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Face_handle Face;

struct FaceDesc {
  bool operator() (const std::pair<Face, K::FT>& p1, const std::pair<Face, K::FT>& p2) const {
    return p1.second < p2.second;
  }
};

void testcase(int n) {
  std::vector<P> infected(n);
  
  for (int i = 0; i < n; ++i) {
    int x, y; std::cin >> x >> y;
    infected[i] = P(x, y);
  }
  
  Triangulation t;
  t.insert(infected.begin(), infected.end());
  
  std::priority_queue<std::pair<Face, K::FT>, std::vector<std::pair<Face, K::FT>>, FaceDesc> internal_faces;
  
  for (auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
    if (t.is_infinite(f)) {
      for (int i = 0; i < 3; ++i) {
        auto fn = f->neighbor(i);
        if (!t.is_infinite(fn)) {
          internal_faces.push(std::make_pair(fn, t.segment(f, i).squared_length()));
        }
      }
    } else f->info() = 0; // initialize all finite faces to 0
  }
  
  while (!internal_faces.empty()) {
    Face f = internal_faces.top().first;
    K::FT d = internal_faces.top().second;
    internal_faces.pop();
    
    if (f->info() != 0) continue; // already has an outgoing point
    f->info() = d;
    
    for (int i = 0; i < 3; ++i) {
      auto fn = f->neighbor(i);
      if (!t.is_infinite(fn) && !fn->info()) {
        internal_faces.push(std::make_pair(fn, std::min(d, t.segment(f, i).squared_length())));
      }
    }
  }
  
  int m; std::cin >> m;
  for (int i = 0; i < m; ++i) {
    int x, y; long d; std::cin >> x >> y >> d;
    P p(x, y);
    if (CGAL::squared_distance(t.nearest_vertex(p)->point(), p) < d) {
      std::cout << 'n';
      continue;
    }
    auto f = t.locate(p);
    if (t.is_infinite(f) || f->info() >= d*4) std::cout << 'y';
    else std::cout << 'n';
  }
  std::cout << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n != 0) {
    testcase(n);
    std::cin >> n;
  }
}