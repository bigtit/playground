#include <stdio.h>

double median(int* s, int n){
  if(n&1) return s[n/2];
  else return (s[n/2]+s[n/2+1])/2;
}

double medians(int* s1, int n1, int* s2, int n2){
  if(!n1) return median(s2, n2);
  else if(!n2) return median(s1, n1);
  int m1 = median(s1, n1);
  int m2 = median(s2, n2);
  if(m1<m2){}
  else if(m1>m2){}
  else return m1;
}
