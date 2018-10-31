#include <iostream>

int guess(int t, int n) {
  if(t>n) return -1;
  int i = 0, j = n/2, m = n/4;

  for(; 1<m && j<n; ++i, m=(m+1)/2) {
    std::cout << j+m << std::endl;
    if(j+m<t) j += m;
  }
  if(1==m || j==n) {
    std::cout << j+m << std::endl;
    return i;
  }
  return -1;
}
int guess2(int t) {
  int g = 1;
  size_t c = 0;
  for(; g<t; ++c, g*=2) { std::cout << g << std::endl; }
  std::cout << g << std::endl << std::endl;
  c += guess(t, g);

  return c;
}
// double version, few differences
int guessd(double t) {
  double g = 1.0f;
  size_t c = 0;
  // make sure g>t and g/2<t
  for(; g<t; ++c, g*=2) { std::cout << g << std::endl; }
  for(; g/2>t; ++c, g/=2) { std::cout << g << std::endl; }
  double j = g/2, m = g/4;
  for(; 1e-4<m && g-j>1e-4; ++c, m/=2) {
    std::cout << j+m << std::endl;
    if(j+m<t) j += m;
  }
  std::cout << j+m << std::endl;
  return c;
}

int main() {
  std::cout << guess2(635343535) << std::endl;;
  std::cout << guessd(54.3453) << std::endl;
  return 0;
}
