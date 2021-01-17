#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// int floor_to_double(const CGAL::Quotient<ET> x) {
  
// }

void testcase(int n, int m) {
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  
  for (int i = 0; i < n; ++i) {
    int min, max; std::cin >> min >> max;
    lp.set_b(i, max);
    lp.set_b(i+n, -min);
  }
  
  for (int i = 0; i < m; ++i) {
    int p; std::cin >> p;
    lp.set_c(i, p);
    for (int j = 0; j < n; ++j) {
      int ci; std::cin >> ci;
      lp.set_a(i, j, ci);
      lp.set_a(i, j+n, -ci);
    }
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_infeasible()) std::cout << "No such diet.\n";
  else std::cout << s.objective_value().numerator() / s.objective_value().denominator() << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int n, m; std::cin >> n >> m;
  while(n != 0) {
    testcase(n, m);
    std::cin >> n >> m;
  }
  
  return 0;
}