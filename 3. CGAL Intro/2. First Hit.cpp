#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <algorithm>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;

long floor_to_double(const K::FT& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return (long)a;
}

void testcase(int n) {
  long x, y, a, b; std::cin >> x >> y >> a >> b;
  P rs(x, y), rp(a, b); // ray source and ray point
  std::vector<S> segs(n);
  
  for (int i = 0; i < n; ++i) {
    long r, s, t, u; std::cin >> r >> s >> t >> u;
    segs[i] = S(P(r, s), P(t, u));
  }
  
  std::random_shuffle(segs.begin(), segs.end());
  
  bool intersects = false;
  P closest; // when an intersection is found, we can just keep the closest point and compare future intersections to a segment
  for (int i = 0; i < n; ++i) {
    // check if intersects
    if (intersects) {
      // check intersection with shortened ray segment
      if (CGAL::do_intersect(S(rs, closest), segs[i])) {
        auto o = CGAL::intersection(S(rs, closest), segs[i]);
        if (const P* op = boost::get<P>(&*o)) {
          // if the intersection is a point, just create a segment until that point
          closest = *op;
        } else if (const S* os = boost::get<S>(&*o)) { // can only be a segment otherwise
          // take end point closer to ray source
          if (CGAL::has_smaller_distance_to_point(rs, os->source(), os->target())) {
            closest = os->source();
          } else {
            closest = os->target();
          }
        } else throw std::runtime_error("strange ray-segment intersection");
      }
    } else {
      // check intersection with original ray
      if (CGAL::do_intersect(R(rs, rp), segs[i])) {
        intersects = true;
        auto o = CGAL::intersection(R(rs, rp), segs[i]);
        if (const P* op = boost::get<P>(&*o)) {
          // if the intersection is a point, just create a segment until that point
          closest = *op;
        } else if (const S* os = boost::get<S>(&*o)) { // can only be a segment otherwise
          // take end point closer to ray source
          if (CGAL::has_smaller_distance_to_point(rs, os->source(), os->target())) {
            closest = os->source();
          } else {
            closest = os->target();
          }
        } else throw std::runtime_error("strange ray-segment intersection");
      }
    }
  }
  
  if (intersects) std::cout << floor_to_double(closest.x()) << " " << floor_to_double(closest.y()) << '\n';
  else std::cout << "no\n";
  
}

int main() {
  int n; std::cin >> n;
  while (n != 0) {
    testcase(n);
    std::cin >> n;
  }
}