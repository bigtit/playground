#include <stdio.h>

int main() {
  unsigned char success;
  unsigned long v;
  asm volatile("rdrand %0;"
               "setc %1"
               : "=r"(v), "=qm"(success));
  printf("success: %i value: %lx\n", success, v);
}
