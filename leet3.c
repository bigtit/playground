// 4ms/981 samples version
#include <stdio.h>
#include <string.h>

int longest_sub(char* s){
  int a[128] = {-1};
  memset(a, -1, sizeof(int)*128);
  int i, j, res, ress;
  i = j = res = ress = 0;
  while(s[j]){
    int c = a[s[j]];
    if(c>=i){
      if(res>ress) ress = res;
      res = res+i-c-1;
      i = c+1;
    }
    ++res;
    a[s[j]] = j;
    ++j;
  }

  return res>ress?res:ress;
}
int main(){
  char s[] = "abcbcabcbcabc";
  printf("%d", longest_sub(s));
  return 0;
}
