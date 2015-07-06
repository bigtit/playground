#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int dic(vector<int>& c)
{
  int i = c.size()-1, j = c.size();
  while(i-- && c[i]>c[i+1]);
  if(i<0) return 1;
  while(--j>i && c[i]>c[j]);
  swap(c[i], c[j]);
  reverse(c.begin()+i+1, c.end());
  return 0;
}

bool isdsp(vector<int>& c)
{
  auto it = c.begin();
  int i = 0;
  while(it!=c.end() && *it!=i++) ++it;
  if(it==c.end()) return true;
  return false;
}

int mindsp(vector<int>& c)
{
  sort(c.begin(), c.end());
  vector<int>::size_type i = 0;
  while(i<c.size()-1){
    swap(c[i], c[i+1]);
    i += 2;
  }
  if(c.size()/2) swap(c[i-1], c[i]);
  return 0;
}
int maxdsp(vector<int>& c)
{
  sort(c.begin(), c.end(), [](int x, int y)->bool{return x>y;});
  if(c.size()/2) swap(c[c.size()/2], c[c.size()/2+1]);
  return 0;
}

int nextdsp(vector<int>& c){
  //
}

int main()
{
  int c0[] = {0,1,2,3,4,5,6};
  vector<int> c (c0, c0+6);
  int c1 = 1, c2 = 0;
  bool is;
  while(!dic(c)){
    ++c1;
    is = isdsp(c);
    c2 += is;
    cout << is << "|";
    for(int j=0; j<c.size(); ++j) cout << c[j];
    cout << endl;
  }
  cout << c1 << endl;
  cout << c2 << endl;
  mindsp(c);
  cout << "minimal dislocation permutation: ";
  for(int i=0; i<c.size(); ++i) cout << c[i];
  cout << endl;
  maxdsp(c);
  cout << "maximal dislocation permutation: ";
  for(int i=0; i<c.size(); ++i) cout << c[i];
  cout << endl;
  return 0;
}
