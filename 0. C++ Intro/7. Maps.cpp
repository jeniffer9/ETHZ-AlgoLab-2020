#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

// using multimap
void testcase_mm() {
  std::multimap<std::string, int> m;
  int q; std::cin >> q;

  for (int i = 0; i < q; ++i) {
    int a; std::cin >> a;
    std::string b; std::cin >> b;
    if (a == 0) {
      m.erase(b);
    } else {
      m.insert(std::make_pair(b, a));
    }
  }

  std::string s; std::cin >> s;
  auto it = m.find(s);
  if (it == m.end()) {
    std::cout << "Empty\n";
  } else {
    std::vector<int> v;
    for (; it->first == s; ++it) {
      v.push_back(it->second);
    }
    std::sort(v.begin(), v.end());
    for (int e: v) {
      std::cout << e << " ";
    }
    std::cout << "\n";
  }
}

// using normal map with a value type of set
void testcase() {
  std::map<std::string, std::set<int>> m;
  int q; std::cin >> q;
  
  for (int i = 0; i < q; ++i) {
    int a; std::cin >> a;
    std::string b; std::cin >> b;
    if (a == 0) {
      m.erase(b);
    } else {
      m[b].insert(a); // m[b] initiates an empty set if it doesn't exist yet
    }
  }
  
  std::string s; std::cin >> s;
  auto sv = m[s];
  if (sv.empty()) {
    std::cout << "Empty\n";
  } else {
    for (auto i : sv) {
      std::cout << i << " ";
    }
    std::cout << "\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
  // while (t--) testcase_mm();
}