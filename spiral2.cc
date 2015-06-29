// Tail-Recursive way
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

bool func(vector<int>& a, vector<vector<int>>& s){
  if(!s.size()) return true;
  if(s.size()==1){ // one line
    for(int i:s[0]) a.push_back(i);
    return true;
  }
  if(s[0].size()==1){ // one column
    for(vector<int> i:s) a.push_back(i[0]);
    return true;
  }
  int m = s.size();
  int n = s[0].size();
  // add boundary
  for(int i=0; i<n; ++i) a.push_back(s[0][i]);
  for(int i=1; i<m; ++i) a.push_back(s[i][n-1]);
  for(int i=2; i<=n; ++i) a.push_back(s[m-1][n-i]);
  for(int i=2; i<m; ++i) a.push_back(s[m-i][0]);
  // create new inner matrix
  if(n<3) return true; // no s left
  vector<vector<int>> t;
  for(int i=1; i<m-1; ++i){
    vector<int> tt(s[i].begin()+1, s[i].end()-1);
    t.push_back(move(tt));
  }
  s = move(t);
  // tail recursive
  return func(a, s);
}

vector<int> spiral(vector<vector<int>>& s){
  vector<int> a;
  func(a, s);
  return move(a);
}

int main(int argc, char** argv){
  vector<vector<int>> a;
  cout << a.size();
};
