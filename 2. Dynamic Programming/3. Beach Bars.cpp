#include <iostream>
#include <vector>
#include <algorithm>

void testcase() {
  int n; std::cin >> n;
  std::vector<int> p(n);
  
  for (int i = 0; i < n; ++i) {
    std::cin >> p[i];
  }
  std::sort(p.begin(), p.end());
  
  int np = 1;
  int solutions = 1;
  int max_dist = 0;
  std::vector<int> min_v = {p[0]};
  std::vector<int> max_v = {p[0]};
  
  for (int i = 1; i < n; ++i) {
    if (p[i-np] + 200 >= p[i]) {
      solutions = 1;
      min_v = {p[i-np]};
      max_v = {p[i]};
      max_dist = (p[i] - p[i-np] + 1) / 2; // +1 to make sure that it rounds up numbers in between integers
      ++np;
    } else if (p[i-np+1] + 200 >= p[i]) {
      int dist = (p[i] - p[i-np+1] + 1) / 2;
      if (dist < max_dist) { // same number of parasols
        solutions = 1;
        min_v = {p[i-np+1]};
        max_v = {p[i]};
        max_dist = dist;
      } else if (dist == max_dist) {
        ++solutions;
        min_v.push_back(p[i-np+1]);
        max_v.push_back(p[i]);
      }
    }
  }
  
  std::cout << np << " " << max_dist << "\n";
  for (int i = 0; i < solutions; ++i) {
    if (min_v[i] + max_dist != max_v[i] - max_dist) {
      std::cout << max_v[i] - max_dist << " " << min_v[i] + max_dist << " ";
    } else {
      std::cout << max_v[i] - max_dist << " ";
    }
  }
  std::cout << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
}