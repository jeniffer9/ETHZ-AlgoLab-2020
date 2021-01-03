#include <iostream>
#include <vector>

void testcase() {
    int n, k; std::cin >> n >> k;
    std::vector<int> cards(n+1, 0); // buffer to avoid out of bound when increasing last index position

    for (int i = 0; i < n; ++i) {
        std::cin >> cards[i];
    }

    int sol_i = 0, sol_j = 0, i = 0, j = 0;
    int curr_sum = cards[0], best = cards[0];
    bool found = false;
    
    while (i < n && j < n && !found) {
      if (curr_sum == k) {
        sol_i = i; sol_j = j;
        found = true;
      } else if (curr_sum > k) {
        if (curr_sum < best) { // closer to k
          sol_i = i; sol_j = j;
        }
        if (i == j) { // don't make i skip j
          curr_sum = cards[++i];
          j = i;
        } else curr_sum -= cards[i++]; // subtract most left card value and increase i
      } else { // curr_sum is smaller than k
        if (curr_sum > best) { // closer to k
          sol_i = i; sol_j = j;
        }
        curr_sum += cards[++j]; // increase j and add next card
      }
    }
    
    std::cout << sol_i << " " << sol_j << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t--) testcase();
}
