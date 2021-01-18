#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

typedef K::Point_2 P;

long ceil_to_long(const EK::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return (long)a;
}

EK::FT to_t(const EK::FT& d2) {
  return CGAL::sqrt((CGAL::sqrt(d2)-1)/2);
}

void testcase(int n) {
  long l, b, r, t; std::cin >> l >> b >> r >> t;
  std::vector<P> bacteria(n);

  for (int i = 0; i < n; ++i) {
    long x, y; std::cin >> x >>y;
    bacteria[i] = P(x, y);
  }
  
  Triangulation tr;
  tr.insert(bacteria.begin(), bacteria.end());
  
  std::vector<double> squared_d(n); // minimum squared distance to next border/gem 
  
  int vi = 0;
  for (auto v = tr.finite_vertices_begin(); v != tr.finite_vertices_end(); ++v) {
    P p = v->point();
    // border distances
    double vert_min_dist = std::min(t-p.y(), p.y()-b);
    double hor_min_dist = std::min(r-p.x(), p.x()-l);
    double border_dist = std::min(vert_min_dist, hor_min_dist);

    squared_d[vi] = 4*border_dist*border_dist;
    
    auto vc = tr.incident_vertices(v);
    P closest_bacterium;
    if (vc != 0) {
      closest_bacterium = vc->point();
      while (++vc != tr.incident_vertices(v)) {
        if (!tr.is_infinite(vc) && CGAL::has_smaller_distance_to_point(p, vc->point(), closest_bacterium)) {
          closest_bacterium = vc->point();
        }
      }
      squared_d[vi] = std::min(squared_d[vi], CGAL::squared_distance(p, closest_bacterium));
    }
    ++vi;
  }
  
  std::sort(squared_d.begin(), squared_d.end());
  long first = ceil_to_long(to_t(squared_d[0]));
  long med = ceil_to_long(to_t(squared_d[n/2]));
  long last = ceil_to_long(to_t(squared_d[n-1]));
  
  std::cout << first << ' ' << med << ' ' << last << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n > 0) {
    testcase(n);
    std::cin >> n;
  }
}