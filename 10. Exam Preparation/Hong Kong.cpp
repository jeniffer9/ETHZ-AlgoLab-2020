#include <iostream>
#include <queue>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h> // exact construction cause square radius won't fit in double
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Circle_2 C;

typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Face_handle F;

const K::FT MAX = K::FT(std::numeric_limits<long>::max()) * K::FT(std::numeric_limits<long>::max());

struct PQ {
  bool operator() (const std::pair<F, K::FT>& p1, const std::pair<F, K::FT>& p2) const {
      return p1.second < p2.second;
  }
};

void testcase() {
  int n, m; long r; std::cin >> n >> m >> r;
  
  std::vector<P> trees(n);
  for (int i = 0; i < n; ++i) {
    long x, y; std::cin >> x >> y;
    trees[i] = P(x,y);
  }
  
  Triangulation t;
  t.insert(trees.begin(), trees.end());
  
  std::priority_queue<std::pair<F, K::FT>, std::vector<std::pair<F, K::FT>>, PQ> que;// face index, max radius to get out
  
  // for each face, get max squared radius that balloon can have to fly away
  for (auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
    f->info() = 0; // initialize all to 0
    if (t.is_infinite(f)) {
      que.push({f, MAX});
    } else {
      C circle(f->vertex(0)->point(), f->vertex(1)->point(), f->vertex(2)->point());
      que.push({f, circle.squared_radius()}); // max radius without touching trees
    }
  }
  
  while(!que.empty()) {
    auto f = que.top().first;
    auto d = que.top().second;
    que.pop();
    if (f->info() != 0) continue; // if already processed continue
    f->info() = d; // max radius it can have
    for (int i = 0; i < 3; ++i) {
      F neighbor = f->neighbor(i);
      if (!t.is_infinite(neighbor) && neighbor->info() == 0) { // update neighbors possibilities
        que.push({neighbor, std::min(d, t.segment(f,i).squared_length())}); // min of current face & entrance to this face
      }
    }
  }

  for (int i = 0; i < m; ++i) {
    long x, y, s; std::cin >> x >> y >> s;
    P b(x,y);
    P nt = t.nearest_vertex(b)->point(); // get nearest tree for balloon
    
    F f = t.locate(b);
    K::FT allowed_dist = K::FT(s)+K::FT(r);
    allowed_dist = allowed_dist*allowed_dist; // allowed distance for inflating
    
    K::FT bd = allowed_dist*4; // allowed distance for take off
    
    K::FT dist = CGAL::squared_distance(nt, b); // distance to nearest tree
    
    if (dist < allowed_dist || f->info() < bd) std::cout << 'n'; // not possible to inflate/ take off
    else std::cout << 'y';
  }
  std::cout << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}