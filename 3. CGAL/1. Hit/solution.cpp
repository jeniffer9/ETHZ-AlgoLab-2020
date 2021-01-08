#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;

void testcase(int n) {
  long x, y, a, b; std::cin >> x >> y >> a >> b;
  R ray(P(x, y), P(a, b));
  std::vector<S> segs(n);
  
  for (int i = 0; i < n; ++i) {
    // P rs, tu; std::cin >> rs >> tu is possible but SLOWER
    long r, s, t, u; std::cin >> r >> s >> t >> u;
    segs[i] = S(P(r, s), P(t, u));
  }
  
  bool intersects = false;
  for (int i = 0; i < n && !intersects; ++i) {
    if (CGAL::do_intersect(ray, segs[i])) {
      intersects = true;
    }
  }
  
  if (intersects) std::cout << "yes\n";
  else std::cout << "no\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n != 0) {
    testcase(n);
    std::cin >> n;
  }
}