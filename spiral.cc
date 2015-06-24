// matrix spiral - clockwise from (0,0)
// 1 2 3
// 4 5 6  -> 1 2 3 6 9 8 7 4 5
// 7 8 9
//
#include <iostream>
#include <vector>
using std::vector;
using std::move;
using std::cout;

vector<int> spiral(vector<vector<int>>& s){
  // null vector
  if(0==s.size()){
    vector<int> res;
    res.clear();
    return move(res);
  }
  if(s.size()==1){ // one line
    return move(s[0]);
  }
  if(s[0].size()==1){ // one column
    vector<int> res;
    for(vector<int> v:s) res.push_back(v[0]);
    return move(res);
  }
  vector<int> res, rres;
  vector<vector<int>> tmp;
  int m = s.size();
  int n = s[0].size();
  for(int i=0; i<n; ++i) res.push_back(s[0][i]);
  for(int i=1; i<m; ++i) res.push_back(s[i][n-1]);
  // reverse the remained matrix
  for(int i=m-1; i>0; --i){
    vector<int> ttmp(s[i].rbegin()+1, s[i].rend());
    tmp.push_back(move(ttmp));
  }
  rres = spiral(tmp);

  res.insert(res.end(), rres.begin(), rres.end());
  return move(res);
}

int main(int argc, char** argv){
  vector<vector<int>> s;
  for(int i=0; i<10; ++i){
    vector<int> tmp;
    for(int j=0; j<11; ++j)
      tmp.push_back(11*i+j);
    s.push_back(move(tmp));
  }
  vector<int> t = spiral(s);
  for(int i:t)
    cout << i << " ";
  cout << " \n" << t.size();
  return 0;
}

