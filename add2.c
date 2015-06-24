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
  int carry = 0;
  int sum = 0;
  ListNode* prev;
  ListNode hdr;
  hdr.next = 0;
  hdr.val = 0;
  prev = &hdr;
  while(a && b){
    ListNode* res = (ListNode*)malloc(sizeof(ListNode));
    res->next = 0;
    sum = carry+a->val+b->val;
    res->val = sum%10;
    carry = sum/10;
    prev->next = res;
    prev = res;
    a = a->next;
    b = b->next;
  }
  while(a){
    sum = a->val+carry;
    ListNode* res = (ListNode*)malloc(sizeof(ListNode));
    res->next = 0;
    res->val = sum%10;
    carry = sum/10;
    prev->next = res;
    prev = res;
    a = a->next;
  }
  while(b){
    sum = b->val+carry;
    ListNode* res = (ListNode*)malloc(sizeof(ListNode));
    res->next = 0;
    res->val = sum%10;
    carry = sum/10;
    prev->next = res;
    prev = res;
    b = b->next;
  }
  if(carry){
    ListNode* res = (ListNode*)malloc(sizeof(ListNode));
    res->next = 0;
    res->val = carry;
    prev->next = res;
    prev = res;
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
