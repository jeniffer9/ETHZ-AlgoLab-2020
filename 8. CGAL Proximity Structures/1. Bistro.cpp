#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

typedef K::Point_2 P;
typedef EK::Point_2 EP;

void testcase(int n) {
  std::vector<K::Point_2> restis(n);
  
  for (int i = 0; i < n; ++i) {
    int x, y; std::cin >> x >> y;
    restis[i] = K::Point_2(x, y);
  }
  
  Triangulation t;
  t.insert(restis.begin(), restis.end());
  
  int m; std::cin >> m;
  for (int i = 0; i < m; ++i) {
    int x, y; std::cin >> x >> y;
    Triangulation::Vertex_handle v = t.nearest_vertex(P(x, y));
    std::cout << CGAL::squared_distance(EP(x, y), EP(v->point().x(), v->point().y())) << "\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  std::cout << std::setprecision(0) << std::fixed;
  while (n > 0) {
    testcase(n);
    std::cin >> n;
  }
}