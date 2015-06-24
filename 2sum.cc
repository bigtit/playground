// given a vector<int> and a int t
// find 2 indices of numbers whose sum of their values equals to t
// nlgn
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

vector<int> twoSum(vector<int>&s, int t){
  vector<struct me> tmp;
  vector<int> res = {0, 0};
  for(int i=0; i<s.size(); ++i){
    struct me a = {i, s[i]};
    tmp.push_back(move(a));
  }
  sort(tmp.begin(), tmp.end(), comp);
  int sum;
  for(int i=0, j=tmp.size()-1; i<j;){
    sum = tmp[i].val+tmp[j].val;
    if(sum>t) --j;
    else if(sum<t) ++i;
    else{
      if(tmp[i].idx<tmp[j].idx){
        res[0] = tmp[i].idx+1;
        res[1] = tmp[j].idx+1;
      }
      else{
        res[0] = tmp[j].idx+1;
        res[1] = tmp[i].idx+1;
      }
      break;
    }
  }

  return move(res);
}

int main(){
  vector<int> s = {3,2,4,15};
  vector<int> a = twoSum(s, 6);
  for(int i:a) cout << i << "\n";
  return 0;
}

