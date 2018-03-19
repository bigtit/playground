#include <iostream>
#include <vector>

// the procedure can be abstract as a guess number game:
// find out the number by just a unidirectional comparison which only returns true or false

// check_seq(n) means for an action seq from number 0 to n-1
// if the critical action number num is under n,
// we will do check_seq(n/2)
class check_seq {
  int num; // internal
public:
  check_seq(int n): num(n) { }
  bool operator()(int n) const { return num < n; }
};

// input n: seq number 0 to n-1
// return the num inside ccq
int binary_check(const check_seq& ccq, int n) {
  if(!ccq(n)) return -1;
  int i = 0, j = 0, m = n; // offset
  // need round
  while(1<m && j<n) {
    m = (m+1)/2;
    if(!ccq(j+m)) j += m;
    ++i;
    std::cout << "m=" << m << ", " << "j=" << j << std::endl;
  }
  if(1==m || j==n) return j;
  return -1;
}

int main(int argc, char** argv) {
  check_seq ccq(6553);
  int r = binary_check(ccq, 69535);
  std::cout << "result:" << r << std::endl;
  return 0;
}
