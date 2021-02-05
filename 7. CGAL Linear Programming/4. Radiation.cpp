#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpz IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Coords {
  IT x;
  IT y;
  IT z;
  
  Coords operator *(const Coords other) {
    return Coords{x*other.x, y*other.y, z*other.z};
  }
};

int h, t;
std::vector<Coords> healthy;
std::vector<Coords> tumorous;
std::vector<std::vector<Coords>> powers;

IT power(IT c, int d) {
  IT sol = 1;
  while(d--) sol *= c;
  return sol;
}

bool solve(int d) {
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  
  for (int hi = 0; hi < h; ++hi) {
    powers[hi][d] = powers[hi][d-1] * healthy[hi];
    int variable = 0;
    for (int x = 0; x <= d; ++x) {
      for (int y = 0; y <= d-x; ++y) {
        for (int z = 0; z <= d-x-y; ++z) {
          lp.set_a(variable++, hi, powers[hi][x].x * powers[hi][y].y * powers[hi][z].z);
          lp.set_b(hi, -1);
        }
      }
    }
  }
  
  for (int ti = 0; ti < t; ++ti) {
    powers[ti+h][d] = powers[ti+h][d-1] * tumorous[ti];
    int variable = 0;
    for (int x = 0; x <= d; ++x) {
      for (int y = 0; y <= d-x; ++y) {
        for (int z = 0; z <= d-x-y; ++z) {
          lp.set_a(variable++, ti+h, -powers[ti+h][x].x * powers[ti+h][y].y * powers[ti+h][z].z);
          lp.set_b(ti+h, -1);
        }
      }
    }
  }
  CGAL::Quadratic_program_options options;
  options.set_pricing_strategy(CGAL::QP_BLAND);
  Solution s = CGAL::solve_linear_program(lp, ET(), options);
  if (!s.is_infeasible()) return true;
  return false;
}

void testcase() {
  std::cin >> h >> t;
  
  healthy = std::vector<Coords>(h);
  tumorous = std::vector<Coords>(t);
  
  powers = std::vector<std::vector<Coords>>(h+t, std::vector<Coords>(31));
  
  for (int i = 0; i < h; ++i) {
    int x, y, z; std::cin >> x >> y >> z;
    healthy[i] = Coords{x, y, z};
    powers[i][0] = Coords{1, 1, 1};
  }
  
  for (int i = 0; i < t; ++i) {
    int x, y, z; std::cin >> x >> y >> z;
    tumorous[i] = Coords{x, y, z};
    powers[h+i][0] = Coords{1, 1, 1};
  }
  
  if (h == 0 || t == 0) {
    std::cout << 0 << '\n';
    return;
  }

  int d = 0;
  while (++d <= 30) {
    if (solve(d)) {
      std::cout << d << '\n';
      return;
    }
  }
  
  std::cout << "Impossible!\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int N; std::cin >> N;
  while(N--) testcase();
}