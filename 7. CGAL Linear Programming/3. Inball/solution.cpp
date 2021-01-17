#include <iostream>
#include <cmath>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(int n) {
  int d; std::cin >> d;

  // ball border point = c + (ai/||ai||)*r (c has d dimensions)
  // substitute ball border points for x in inequalities
  // -> ai^T * x <= bi   =   ai^T * (c + (ai/||ai||)*r) <= bi   =   ai^T * c + ||ai|| * r <= bi
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  
  const int r = 0;
  lp.set_l(r, true, 0); // set lower bound of r to 0
  
  for (int i = 0; i < n; ++i) {
    int norm = 0;
    for (int j = 0; j < d; ++j) { // ai^T * c
      int aj; std::cin >> aj;
      lp.set_a(j+1, i, aj);
      norm += aj*aj;
    }
    lp.set_a(r, i, (int)std::sqrt(norm)); // ||ai|| * r
    int b; std::cin >> b;
    lp.set_b(i, b);
  }
  
  lp.set_c(r, -1);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  if (s.is_infeasible()) {
    std::cout << "none\n";
  } else if (s.is_unbounded()) {
    std::cout << "inf\n";
  } else {
    auto sol = s.objective_value();
    std::cout << -sol.numerator() / sol.denominator() << "\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int n; std::cin >> n;
  while(n > 0) {
    testcase(n);
    std::cin >> n;
  }
}