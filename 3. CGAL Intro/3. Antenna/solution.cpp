#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 P;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt SK;
typedef CGAL::Min_circle_2_traits_2<SK> STraits;
typedef CGAL::Min_circle_2<STraits> SMin_circle;
typedef SK::Point_2 SP;

long ceil_to_double(const SK::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return (long)a;
}

void testcase(int n) {
  std::vector<P> homes(n);
  for (int i = 0; i < n; ++i) {
    double x, y; std::cin >> x >> y;
    homes[i] = P(x, y);
  }
  
  if (n == 1) { // radius is 0 if there's only one point
    std::cout << 0 << '\n';
    return;
  }
  
  Min_circle mc(homes.begin(), homes.end(), true); // build enclosing circle with cheaper Kernel
  int nsp = mc.number_of_support_points(); // #support points; <=3
  std::vector<SP> support_points(nsp); // convert support points into sqrt Kernel
  for (int i = 0; i < nsp; ++i) {
    support_points[i] = SP(CGAL::to_double(mc.support_point(i).x()), CGAL::to_double(mc.support_point(i).y()));
  }

  SMin_circle smc(support_points.begin(), support_points.end(), false); // construct exact circle with only the support points
  STraits::Circle c = smc.circle();
  
  std::cout << ceil_to_double(CGAL::sqrt(c.squared_radius())) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n != 0) {
    testcase(n);
    std::cin >> n;
  }
}
