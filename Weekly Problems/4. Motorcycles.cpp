#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;

struct B {
  int ix;
  long start;
  K::FT slope;
  
  bool operator <(const B& bo) {
    return start < bo.start;
  }
};

void testcase() {
  int n; std::cin >> n;
  std::vector<B> bikers(n);
  
  for (int i = 0; i < n; ++i) {
    long y0, x, y1; std::cin >> y0 >> x >> y1;
    K::FT slope = K::FT(y1-y0)/K::FT(x);
    bikers[i] = {i, y0, slope};
  }
  
  std::sort(bikers.begin(), bikers.end());
  
  std::stack<B> st;
  st.push(bikers[0]);
  
  for (int i = 1; i < n; ++i) {
    const B curr = bikers[i];
    if (curr.slope >= 0) { // if positive slope, remove all below with steeper slope
      while (!st.empty() && st.top().slope > curr.slope) st.pop();
      //std::cout << "adding here " << curr.ix << " w slope " << curr.slope << '\n';
      st.push(curr);
    } else { // if negative slope
      if (st.top().slope <= 0) { // if below slope is also negative
        if (st.top().slope > curr.slope) continue; // but less steep, skip current
        else st.push(curr); // if steeper, add current
      } else { // if other slope is positive
        while (!st.empty() && st.top().slope > -curr.slope) st.pop(); // remove all bikers that have a steeper slope
        if (st.empty()) st.push(curr); // only insert current if stack is empty
        else if (st.top().slope < 0) --i; // else check again if there are negative slopes below all positive
      }
    }
  }
  
  std::vector<int> sol;
  while(!st.empty()) {
    sol.push_back(st.top().ix);
    st.pop();
  }
  
  std::sort(sol.begin(), sol.end());
  for (int i : sol) {
    std::cout << i << " ";
  }
  std::cout << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}