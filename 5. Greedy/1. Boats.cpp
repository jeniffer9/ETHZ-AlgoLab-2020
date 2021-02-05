#include <iostream>
#include <vector>
#include <algorithm>

struct Boat {
  int p;
  int l;
  
  bool operator <(const Boat& other) {
    return p < other.p;
  }
};

void testcase() {
  int n; std::cin >> n;
  std::vector<Boat> boats = std::vector<Boat>(n);
  
  for (int i = 0; i < n; ++i) {
    int p, l; std::cin >> l >> p;
    boats[i] = {p, l};
  }
  
  std::sort(boats.begin(), boats.end());
  
  int f = boats[0].p;
  int result = 1;
  int f_next = -1;
  
  for (int i = 1; i < n; ++i) {
    Boat curr = boats[i];
    if (f > curr.p) continue;
    if (f_next != -1) {
      if (curr.p >= f_next) {
        f = f_next; f_next = -1; --i; // check curr again
      } else if (f+curr.l >= f_next) continue;
      else if (f+curr.l <= curr.p) {// f+curr.l < f_next
        f = curr.p; f_next = -1;
      } else { // f+curr.l < f_next && f+curr.l > curr.p
        f_next = f+curr.l;
      }
    } else { // no f_next
      if (f+curr.l <= curr.p) {
        f = curr.p;
      } else {// f+curr.l > curr.p
        f_next = f+curr.l;
      }
      ++result;
    }
  }
  
  std::cout << result << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}

