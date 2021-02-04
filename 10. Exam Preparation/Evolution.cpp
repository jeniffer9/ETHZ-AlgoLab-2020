#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

// request
struct R {
  int i;
  int b;
};

std::map<std::string, int> s_to_i; // species name to index
std::vector<std::vector<R>> req; // request for each species
std::vector<int> results; // species index results ordered for queries
std::vector<int> age; // age of species
std::vector<std::vector<int>> tree; // direct offspring for each species

// take species and its path
void binary_search(int sp, std::vector<int>& path) { 
  for (auto r : req[sp]) { // iterate though all request for this node
    int l = -1; // we know it's false for l
    int h = (int)path.size()-1; // we know it could be true for h (itself)
    while (l + 1 < h) {
      int mid = (h-l)/2 + l;
      int a = age[path[mid]];
      if (a <= r.b) h = mid;
      else l = mid;
    }
    results[r.i] = path[h]; // save result in result array under the index of the query
  }
}

void dfs(int sp, std::vector<int>& path) {
  if (!req[sp].empty()) { // resolve ther queries if there are any
    binary_search(sp, path);
  }
  for (int child : tree[sp]) { // for all direct offsprings of the current species
    path.push_back(child); // push into path
    dfs(child, path);
  }
  path.pop_back(); // remove the current species when going back
}

void testcase() {
  int n, q; std::cin >> n >> q;
  
  s_to_i.clear();
  std::vector<std::string> species(n);
  age.clear(); age.reserve(n);
  
  for (int i = 0; i < n; ++i) {
    std::string s; int a; std::cin >> s >> a;
    s_to_i[s] = i; // map this name to this index
    species[i] = s; // map this index to this name
    age.emplace_back(a);
  }
  
  int root = std::max_element(age.begin(), age.end()) - age.begin(); // index of root
  
  tree = std::vector<std::vector<int>>(n);
  for (int i = 0; i < n-1; ++i) {
    std::string s, p; std::cin >> s >> p;
    int pi = s_to_i[p]; int si = s_to_i[s];
    tree[pi].push_back(si);
  }
  
  req = std::vector<std::vector<R>>(n);
  for (int i = 0; i < q; ++i) {
    std::string s; int b; std::cin >> s >> b;
    req[s_to_i[s]].push_back(R{i, b});
  }
  
  results = std::vector<int>(q);
  std::vector<int> path; path.push_back(root);
  dfs(root, path);
  
  for (auto s : results) {
    std::cout << species[s] << ' ';
  }
  std::cout << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}