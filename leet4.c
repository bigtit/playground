#include <stdio.h>
#include <stdlib.h>

double median(int* s, int n){
  if(n&1) return s[n/2];
  else return (s[n/2]+s[n/2+1])/2;
}

int bbsearch(int* a, int len, int x){
  if(len<=0) return -1;
  if(a[len/2]==x) return len/2;
  if(a[len/2]>x) return bbsearch(a, len/2, x);
  else return len/2+1+bbsearch(a+len/2, len/2, x);
}

int* bmerge(int* s1, int n1, int* s2, int n2){
  if(!n1) return s2;
  if(!n2) return s1;
  int m1 = median(s1, n1);
  int m2 = median(s2, n2);
  if(m1>m2) return bmerge(s2, n2, s1, n1);
  int* tmp = (int*)malloc(sizeof(int)*(n1+n2));
  if(s2[n2-1]<=s1[n1-1]) return tmp;
  return tmp;
}

double medians(int* s1, int n1, int* s2, int n2){
  if(!n1) return median(s2, n2);
  else if(!n2) return median(s1, n1);
  int m1 = median(s1, n1);
  int m2 = median(s2, n2);
  if(m1<m2){return 0;}
  else if(m1>m2) return medians(s2, n2, s1, n1);
  else return m1;
}
