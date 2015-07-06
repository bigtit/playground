#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int next_perm(vector<int>& c){
  int i = c.size()-1;
  int j = i+1;
  while(i-- && c[i]>c[i+1]);
  if(i<0) return 1; // termination condition: descending orde
  while(--j>i && c[i]>c[j]);
  swap(c[i], c[j]);
  reverse(c.begin()+i+1, c.end());
  return 0;
}

bool isdsp(vector<int>& c){
  auto it = c.begin();
  int i = 0;
  while(it!=c.end() && *it!=i++) ++it;
  if(it==c.end()) return true;
  return false;
}

int mindsp(vector<int>& c){
  sort(c.begin(), c.end());
  vector<int>::size_type i = 0;
  while(i<c.size()-1){
    swap(c[i], c[i+1]);
    i += 2;
  }
  if(c.size()/2) swap(c[i-1], c[i]);
  return 0;
}
int maxdsp(vector<int>& c){
  sort(c.begin(), c.end(), [](int x, int y)->bool{return x>y;});
  if(c.size()/2) swap(c[c.size()/2], c[c.size()/2+1]);
  return 0;
}

int nextdsp(vector<int>& c){
  next_perm(c);
  return 0;
}

int main(){
  vector<int> c = {0,1,2,3,4,5};
  while(!next_perm(c)){
    //cout << is << "|";
    cout << isdsp(c);
    //for(int j=0; j<c.size(); ++j) cout << c[j];
    //for(int i:c) cout << i;
    //cout << endl;
  }

  return 0;
}
