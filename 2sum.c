#include <stdio.h>
#include <stdlib.h>

typedef struct me{
  int idx;
  int val;
}me;

int comp(const void* a, const void* b){
  return ((me*)a)->val - ((me*)b)->val;
}

int* twoSum(int* s, int len, int t){
  int* res = (int*)malloc(sizeof(int)*2);
  me* tmp = (me*)malloc(sizeof(me)*len);
  for(int i=0; i<len; ++i){
    tmp[i].idx = i;
    tmp[i].val = s[i];
  }
  qsort(tmp, len, sizeof(me), comp);
  for(int i=0; i<len; ++i){
    printf("%d,%d\n", tmp[i].idx, tmp[i].val);
  }
  int sum;
  for(int i=0, j=len-1; i<j;){
    sum = tmp[i].val+tmp[j].val;
    if(sum>t) --j;
    else if(sum<t) ++i;
    else{
      if(tmp[i].idx<tmp[j].idx){
        res[0] = tmp[i].idx+1;
        res[1] = tmp[j].idx+1;
      }else{
        res[0] = tmp[j].idx+1;
        res[1] = tmp[i].idx+1;
      }
      break;
    }
  }
  free(tmp);
  return res;
}

int main(){
  int s[] = {3,2,4,15};
  int* a = twoSum(s, 4, 6);
  printf("%d, %d", a[0], a[1]);
  free(a);
  return 0;
}
