#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct me{
  int idx;
  int val;
};

bool comp(const struct me& a, const struct me& b){
  return a.val < b.val;
}

typedef vector<struct me>::iterator mei;
mei binary_find(mei beg, mei end, int val){
  struct me a = {0, val};
  mei i = lower_bound(beg, end, a, comp);
  if(i!=end && val<i->val) return i;
  else return end;
}

vector<int> twoSum(vector<int>&s, int t){
  vector<struct me> tmp;
  vector<int> res;
  for(int i=0; i<s.size(); ++i){
    struct me a = {i, s[i]};
    tmp.push_back(move(a));
  }
  sort(tmp.begin(), tmp.end(), comp);
  for(struct me a:tmp){
    auto b = binary_find(tmp.begin(), tmp.end(), t-a.val);
    if(b!=tmp.end()){
      if(a.idx<b->idx)
        res.push_back(a.idx+1);
      res.push_back(b->idx+1);
      break;
    }
  }
  return move(res);
}

int main(){
  vector<int> s = {2,7,11,15};
  vector<int> a = twoSum(s, 9);
  for(int i:a) cout << i << "\n";
  return 0;
}

