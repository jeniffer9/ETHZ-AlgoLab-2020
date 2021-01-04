#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>

std::vector<int> m;
std::vector<std::vector<int>> p;

void testcase() {
  int n; std::cin >> n;
  m = std::vector<int>(n);
  p = std::vector<std::vector<int>>(n);
  
  for (int i = 0; i < n; ++i) {
    std::cin >> m[i];
    p[i] = std::vector<int>(m[i]);
  }
  
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m[i]; ++j) {
      std::cin >> p[i][j];
    }
  }
  
  int a = p[0][0];
  int b = p[0][0];
  
  for (int i = 0; i < n; ++i) {
    a = std::min(p[i][0], a); // get the minimum value of all first positions
    b = std::max(p[i][m[i]-1], b); // get the max value of all last positions
  }
  
  std::vector<int> ix(n, 0); // keep current index position for each word
  
  int dist = b-a+1;
  bool done = false;
  
  while (!done && dist != n) {
    int min_i = 0, val_min = p[0][ix[0]];
    int val_max = val_min;
    for (int i = 1; i < n; ++i) {
      int value = p[i][ix[i]]; // go though all current indexes for all words
      if (value < val_min) { // keep the very smallest index
        min_i = i; // word index
        val_min = value; // position of word
      } else if (value > val_max) {
        val_max = value;
      }
    }
    
    int d = val_max - val_min + 1;
    if (d < dist) {
      dist = d;
    }
    if (++ix[min_i] == m[min_i]) { // narrow range by increase very smallest index
      done = true; // end as soon as it goes out of bounds
    }
  }
  
  std::cout << dist << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
}