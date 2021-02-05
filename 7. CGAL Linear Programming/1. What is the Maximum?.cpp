#include <iostream>
#include <algorithm>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int floor_to_double(const CGAL::Quotient<ET>& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return (int)a;
}

int ceil_to_double(const CGAL::Quotient<ET>& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return (int)a;
}

void print_sol(Solution* s, int p) {
  if (s->is_unbounded()) {
    std::cout << "unbounded\n";
  } else if (s->is_infeasible()) {
    std::cout << "no\n";
  } else {
    if (p==1) std::cout << floor_to_double(-s->objective_value()) << "\n";
    else std::cout << ceil_to_double(s->objective_value()) << "\n";
  }
}

void minimize(int a, int b) {
  // x, y, z <= 0
  Program lp (CGAL::LARGER, false, 0, true, 0);
  const int X = 0;
  const int Y = 1;
  const int Z = 2;
  
  // x + y >= -4
  lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, -4);
  // 4x + 2y + z >= -ab
  lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a*b);
  // -x + y >= -1
  lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, -1);
  
  lp.set_c(X, a); lp.set_c(Y, b); lp.set_c(Z, 1);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  print_sol(&s, 2);
}

void maximize(int a, int b) {
  Program lp (CGAL::SMALLER, true, 0, false, 0);

  const int X = 0;
  const int Y = 1;
  
  lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);
  lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);
  lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);
  
  lp.set_c(X, a); lp.set_c(Y, -b);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  print_sol(&s, 1);
}

void testcase(int p) {
  int a, b; std::cin >> a >> b;
  
  if (p == 2) minimize(a, b);
  else maximize(a, b);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int p; std::cin >> p;
  while(p > 0) {
    testcase(p);
    std::cin >> p;
  }
  
  return 0;
}