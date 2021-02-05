#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef long IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

IT xs, ys;

int sign(int a, int b, int c) {
  return a*xs + b*ys + c > 0 ? 1 : -1; // return side of lign
}

void testcase() {
  int n; std::cin >> xs >> ys >> n;
  
  Program lp (CGAL::LARGER, false, 0, false, 0);
  
  const int PX = 0;
  const int PY = 1;
  const int T = 2;
  
  for (int i = 0; i < n; ++i) {
    // d(p,l)/v = time it takes until at Asterix
    // ti - T >= 0 for each legion i
    IT a, b, c, v; std::cin >> a >> b >> c >> v;
    int s = sign(a, b, c); // signed distance depends on starting point
    IT det = std::sqrt(a*a+b*b);
    lp.set_a(PX, i, s*a); lp.set_a(PY, i, s*b); lp.set_a(T, i, -v*det);
    lp.set_b(i, -s*c);
  }
  lp.set_l(T, true, 0);
  // maximize T
  lp.set_c(T, -1);
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  std::cout << -s.objective_value().numerator()/s.objective_value().denominator() << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
 }