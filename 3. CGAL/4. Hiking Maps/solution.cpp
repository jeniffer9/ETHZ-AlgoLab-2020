#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

P readPoint() {
  int x, y; std::cin >> x >> y;
  return P(x, y);
}

void testcase() {
  int m, n; std::cin >> m >> n;
  std::vector<P> leg_points(m);
  std::vector<std::vector<int>> map_parts(n); // store the legs a map part covers

  for (int i = 0; i < m; ++i) { // read all leg points
    int x, y; std::cin >> x >> y;
    leg_points[i] = P(x, y);
  }
  
  bool opt_sol = false;
  for (int i = 0; i < n; ++i) { // read all map parts
    P q0 = readPoint(), q1 = readPoint(), q2 = readPoint(), q3 = readPoint(), q4 = readPoint(), q5 = readPoint();
    std::vector<P> qs(6);
    // align the points s.t they have the same orientation from 0-5
    // same orientation = walk from q0 to q5 in order;
    if (CGAL::left_turn(q0, q1, q2)) {qs[0] = q1; qs[1] = q0;}
    else {qs[0] = q0; qs[1] = q1;}
    if (CGAL::left_turn(q2, q3, q4)) {qs[2] = q3; qs[3] = q2;}
    else {qs[2] = q2; qs[3] = q3;}
    if (CGAL::left_turn(q4, q5, q1)) {qs[4] = q5; qs[5] = q4;}
    else {qs[4] = q4; qs[5] = q5;}

    for (int j = 0; j < m-1; ++j) {
      P p1 = leg_points[j], p2 = leg_points[j+1];
      // check that for every side point it is either a right turn or collinear orientation to the two leg points
      if (!CGAL::left_turn(qs[0], qs[1], p1) && !CGAL::left_turn(qs[0], qs[1], p2)
          && !CGAL::left_turn(qs[2], qs[3], p1) && !CGAL::left_turn(qs[2], qs[3], p2)
          && !CGAL::left_turn(qs[4], qs[5], p1) && !CGAL::left_turn(qs[4], qs[5], p2)) {
        map_parts[i].push_back(j);
      }
    }
    // one map part includes all legs = optimal solution
    if ((int)map_parts[i].size() == m-1) opt_sol = true;
  }
  if (opt_sol) {
    std::cout << 1 << '\n';
    return;
  }
  
  int i = 0, j = 1;
  int best = n;
  bool opt = false; // optimal is 2 now
  std::vector<int> covered(m-1); // how many times each leg is covered
  
  int nc = 0; // count how many are covered
  for (int l : map_parts[0]) { covered[l]++; nc++;}
  for (int l : map_parts[1]) {
    if (++covered[l] == 1) ++nc;
  }

  while (i < n && j < n && !opt) {
    if (nc == m-1) { // compare result if it covers all legs
      int c = j-i+1;
      if (c < best) {
        best = c;
        if (c == 2) { // if optimal then finish
          opt = true;
        }
      }
      // advance the index 
      ++i; // remove legs coverex by i
      for (int l : map_parts[i-1]) {
        covered[l]--;
        if (covered[l] == 0) nc--;
      }
    } else {
      ++j;
      if (j < n) {
        for (int l : map_parts[j]) {
          covered[l]++;
          if (covered[l] == 1) nc++;
        }
      }
    }
  }
  
  std::cout << best << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
}