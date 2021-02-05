#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <boost/pending/disjoint_sets.hpp> // union find

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb; // nn
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef K::Point_2 P;
typedef Triangulation::Vertex_handle V;

void testcase() {
  int n, m, k; long s; std::cin >> n >> m >> s >> k;
  
  std::vector<std::pair<P, int>> trees(n);
  for (int i = 0; i < n; ++i) {
    int x, y; std::cin >> x >> y;
    trees[i] = {P(x, y), i};
  }
  
  Triangulation t;
  t.insert(trees.begin(), trees.end());
  
  std::vector<std::tuple<K::FT, int, int>> ds; // distances that connect 2 vertices
  
  // get the distances of all finite edges
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    auto v1 = e->first->vertex((e->second + 1) % 3)->info();
    auto v2 = e->first->vertex((e->second + 2) % 3)->info();
    ds.push_back({t.segment(e).squared_length(), v1, v2});
  }
  
  //std::vector<P> bones(m);
  for (int i = 0; i < m; ++i) {
    int x, y; std::cin >> x >> y;
    P p(x, y);
    auto nv = t.nearest_vertex(p);
    ds.push_back({4*CGAL::squared_distance(nv->point(), p), nv->info(), -1}); // -1 = bone
  }
  
  std::sort(ds.begin(), ds.end());
  boost::disjoint_sets_with_storage<> dis(n);
  std::vector<int> bones(n, 0); // how many bones you can find at each vertex
  
  int a = 0;
  K::FT b = -1;
  for (auto di : ds) {
    K::FT d; int v1, v2;
    std::tie(d, v1, v2) = di;
    
    if (v2 == -1) { // bone
      bones[dis.find_set(v1)] += 1; // add the bone to the set of the nearest vertex
    } else {
      int u = dis.find_set(v1);
      int v = dis.find_set(v2);
      if (u != v) {
        int bs = bones[u] + bones[v];
        // link them
        dis.link(u, v);
        bones[dis.find_set(v1)] = bs;
      }
    }
    if (d <= s) {
      a = std::max(a, bones[dis.find_set(v1)]);
    }
    if (b == -1 && bones[dis.find_set(v1)] >= k) {
      b = d;
    }
  }
  
  std::cout << std::setprecision(0) << std::fixed << a << ' ' << b << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}