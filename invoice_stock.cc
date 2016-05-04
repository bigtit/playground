#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

struct eeds {
  int stock;
  // int fund;
};

struct eds {
  int bat_no; // other entries reserved
  vector<eeds> ee;
};
struct ids {
  int stock;
  int bat_no;
};

int main() {
  vector<eds> m{{1,{{5},{3},{6}}},{2,{{4},{2}}}};
  vector<ids> n{{3,-1},{2,-1},{3,-1},{2,-1},{4,-1},{3,-1},{1,-1},{3,-1}};
  int ni = 0;
  for(auto i : m) {
    cout << "export no: " << i.bat_no << endl;
    for(auto j : i.ee) {
      cout << "stock: " << j.stock << endl;
      int tmp = j.stock;
      while(tmp >= n[ni].stock) {
        tmp -= n[ni].stock;
        n[ni].bat_no = i.bat_no;
        cout << "n[" << ni << "].bat_no = " << n[ni].bat_no << endl;
        ++ni;
      }
      if(tmp != 0 && n[ni].stock > tmp) {
        cout << "error encountered, ni= " << ni << endl;
        exit(-1);
      }
    }
  }
  return 0;
}
