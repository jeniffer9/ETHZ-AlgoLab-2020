#include <iostream>
#include <vector>
#include <iomanip>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

double floor_to_double(CGAL::Quotient<ET> const & x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

void testcase() {
  int n, m, c;
  std::cin >> n >> m >> c;
  
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  
  std::vector<P> w_s(n+m);
  std::vector<int> alc_p(n);
  
  // #0-#n equations for max beer supply of warehouses
  for (int i = 0; i < n; ++i) {
    int x, y, s, a; std::cin >> x >> y >> s >> a;
    w_s[i] = P(x, y);
    alc_p[i] = a;
    lp.set_b(i, s); // max supply of warehouse i / beer i
    for (int s = 0; s < m; ++s) { // for each stadium
      lp.set_a(i*m+s, i, 1); // variable of beer i for each stadium
    }
  }
  
  for (int i = 0; i < m; ++i) {
    int x, y, d, u; std::cin >> x >> y >> d >> u;
    lp.set_b(n+i, u*100); // n - n+m-1 equation stadiums for alcohol
    lp.set_b(n+m+i, d); // n+m - n+2*m-1 equation stadiums for upper demand
    lp.set_b(n+2*m+i, -d); // n+2*m - n+3*m-1 equation for stadiums for lower demand
    w_s[i+n] = P(x, y);
    for (int b = 0; b < n; ++b) { // for each beer
      lp.set_a(b*m+i, n+i, alc_p[b]); // variable of beer b for each stadium, stadium alcohol
      lp.set_a(b*m+i, n+m+i, 1); // stadium demand upper limit
      lp.set_a(b*m+i, n+2*m+i, -1); // stadium demand lower limit
    }
  }
  
  std::vector<std::vector<std::pair<int, int>>> rs(n, std::vector<std::pair<int, int>>(m));
  for (int i = 0; i < n; ++i) { // beer
    for (int j = 0; j < m; ++j) { // stadium
      int rws; std::cin >> rws;
      rs[i][j] = {rws, 0};
    }
  }
  
  Triangulation t;
  t.insert(w_s.begin(), w_s.end());

  for (int i = 0; i < c; ++i) {
    int x, y; long r; std::cin >> x >> y >> r;
    P cp(x, y);
    long sq_r = r*r;
    auto nv = t.nearest_vertex(cp);
    if (CGAL::squared_distance(nv->point(), cp) < sq_r) {
      for (int w = 0; w < n; ++w) {
        bool w_in_c = CGAL::squared_distance(w_s[w], cp) < sq_r;
        for (int s = 0; s < m; ++s) {
          bool s_in_c = CGAL::squared_distance(w_s[n+s], cp) < sq_r;
          if (s_in_c != w_in_c) { // need to pass through this contour line
            rs[w][s].second++; // increase t_ws
          }
        }
      }
    }
  }
  
  for (int i = 0; i < n; ++i) { // beer
    for (int j = 0; j < m; ++j) { // stadium
      lp.set_c(i*m+j, -100*rs[i][j].first+rs[i][j].second); // maximize sum of revenues
    }
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_infeasible()) std::cout << "RIOT!\n";
  else {
    double o = floor_to_double(-s.objective_value() / 100);
    std::cout << std::setprecision(0) << std::fixed << o << '\n';
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}