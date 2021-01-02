#include <iostream>
#include <vector>

typedef std::vector<int> Vec;
typedef std::vector<Vec> Matrix;

// O(n^3) solution requires dimensionality reduction
// reduce 2 dimensional problem to 1 dimension problem
void testcase() {
  int n; std::cin >> n;

  Matrix M(n, Vec(n));
  Matrix subM(n, Vec(n)); // Sum of sub matrices from starting position (1,1)
  
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cin >> M[i][j];
    }
  }
  
  // initialize first row
  subM[0][0] = M[0][0];
  for (int i = 1; i < n; ++i) {
    subM[i][0] = M[i][0] + subM[i-1][0];
  }
  
  // initialize first column
  for (int i = 1; i < n; ++i) {
    subM[0][i] = M[0][i] + subM[0][i-1];
  }
  
  // fill in rest
  for (int i = 1; i < n; ++i) {
    for (int j = 1; j < n; ++j) {
      subM[i][j] = subM[i-1][j] + subM[i][j-1] - subM[i-1][j-1] + M[i][j];
    }
  }
  
  // idea: transform into 1 dimension by collapsing each row range into 1 cell for each y value
  // then solve the even pairs problem across new vertical dimension
  // The final result will be the accummulation of the sub results
  int result = 0;
  // iterate through each possible horizontal range
  for (int i1 = 0; i1 < n; ++i1) {
    for (int i2 = i1; i2 < n; ++i2) {
      Vec rowSum(n); // sum of range (i1, i2) over vertical axis
      
      if (i1 == 0) { // attention with first column
        rowSum[0] = subM[i2][0]; // attention with first row
        for (int j = 1; j < n; ++j) {
          rowSum[j] = subM[i2][j] - subM[i2][j-1];
        }
      } else {
        rowSum[0] = subM[i2][0] - subM[i1-1][0]; // attention with first row
        for (int j = 1; j < n; ++j) {
          rowSum[j] = subM[i2][j] - subM[i1-1][j] - subM[i2][j-1] + subM[i1-1][j-1];
        }
      }
      
      // Even Pairs Algo
      int even = 0, odd = 0, sum = 0;
      for (int i = 0; i < n; ++i) {
        sum += rowSum[i];
        if (sum % 2 == 0) ++even;
        else ++odd;
      }
      
      result += (even + even * (even-1) / 2 + odd * (odd-1) / 2);
    }
  }
  
  std::cout << result << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) testcase();
}