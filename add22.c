#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode{
  int val;
  struct ListNode* next;
}ListNode;

ListNode* add2(ListNode* a, ListNode* b){
  if(!a) return b;
  else if(!b) return a;
  else if(!a && !b) return 0;
  int c = 0;
  int sum = 0;
  int aa, bb;
  ListNode* prev;
  ListNode hdr;
  hdr.next = 0;
  hdr.val = 0;
  prev = &hdr;
  while(a || b || c){
    ListNode* res = (ListNode*)malloc(sizeof(ListNode));
    res->next = 0;
    aa = a?a->val:0;
    bb = b?b->val:0;
    sum = c+aa+bb;
    res->val = sum%10;
    c = sum/10;
    prev->next = res;
    prev = res;
    a = a?a->next:0;
    b = b?b->next:0;
  }

  return hdr.next;
}

int main(){
  ListNode a, b;
  a.next = 0;
  a.val = 5;
  b.val = 6;
  b.next = 0;
  ListNode* c = add2(&a, &b);
  printf("%d", c->val);
  return 0;
}
