#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct Move {
  long d;
  long t;
};

long D, T;

// Split & List Algo from the Slides
bool make_l(std::vector<std::pair<Move, int>>& l, std::vector<Move>& s) {
  std::sort(s.begin(), s.end(), [](const Move& mv1, const Move& mv2) {
    return mv1.t < mv2.t;
  });

  for (int i = 0; i < (1<<(int)s.size()); ++i) {
    long d = 0;
    long t = 0;
    int nm = 0; // number of moves
    for (int j = 0; j < (int)s.size(); ++j) {
      if (i & (1<<j)) { // j-th element in subset
        t += s[j].t; if (t>=T) break;
        d += s[j].d;
        ++nm;
      }
    }
    if (t < T) { // don't include failing solutions
      if (d >= D) return true; // optimal solution found
      l.push_back(std::make_pair(Move{d, t}, nm));
    }
  }
  return false;
}

void testcase() {
  int n, m;
  std::cin >> n >> m >> D >> T;
  
  std::vector<Move> moves;
  std::vector<long> gulps(m);
  
  bool opt = false; // stop when an optimal solution (without gulps) is found

  for (int i = 0; i < n; ++i) {
    long d, t; std::cin >> d >> t;
    if (t < T) { // only save moves that could lead to a solution
      moves.push_back({d, t});
      if (d >= D) opt = true;
    } 
  }
  
  for (int i = 0; i < m; ++i) {
    std::cin >> gulps[i];
  }
  
  if (opt) {
    std::cout << 0 << '\n';
    return;
  }
  
  if (moves.empty()) {
    std::cout << "Panoramix captured\n";
    return;
  }

  n = moves.size();
  m = gulps.size();

  // stores the accumulated move and how many moves are included
  std::vector<Move> s1(moves.begin(), moves.begin()+n/2);
  std::vector<Move> s2(moves.begin()+n/2, moves.end());
  std::vector<std::pair<Move, int>> l1;
  std::vector<std::pair<Move, int>> l2;
  if (make_l(l1, s1) || make_l(l2, s2)) {
    std::cout << 0 << '\n';
    return;
  }

  std::sort(l2.begin(), l2.end(),
            [](const std::pair<Move, int>& p1, const std::pair<Move, int>& p2){
                return p1.first.t < p2.first.t || (p1.first.t == p2.first.t && p1.first.d > p2.first.d);
  });
  
  std::vector<long> dist(n+1, -1); // how far do you get with how many moves

  // //int nm = 1;
  for (int i = 0; i < (int)l1.size(); ++i) {
    Move m1 = l1[i].first;
    int nm1 = l1[i].second;
    for (int j = 0; j < (int)l2.size(); ++j) {
      Move m2 = l2[j].first;
      int nm2 = l2[j].second;
      long optd = m1.d+m2.d;
      if (m1.t + m2.t >= T) break;
      else if (optd >= D) {
        std::cout << 0 << '\n';
        return;
      } else dist[nm1+nm2] = std::max(dist[nm1+nm2], optd);
    }
  }
  
  if (gulps.empty()) {
    std::cout << "Panoramix captured\n";
    return;
  }
  
  double gn = -1; // + dist per move needed
  for (int i = n; i > 0; --i) {
    if (dist[i] > 0) {
      double dleft = D-dist[i];
      if (gn < 0) gn = std::ceil(dleft / i);
      else gn = std::min(gn, std::ceil(dleft / i));
    }
  }

  auto sol = std::lower_bound(gulps.begin(), gulps.end(), (long)gn);
  if (gn > 0 && sol != gulps.end()) std::cout << sol-gulps.begin()+1 << '\n';
  else std::cout << "Panoramix captured\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) testcase();
}