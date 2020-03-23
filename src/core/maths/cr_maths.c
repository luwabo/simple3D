#include "cr_maths.h"

int cr_maths_nextPow2 (int n) {

  if (n <= 0) {
    return 0;
  }

  int pow2 = 1;

  while (pow2 < n) {
    pow2 <<= 1;
  }

  return pow2;
}

int cr_maths_align (int n, int num) {

  return (n % num == 0 ? n : n + num - (n % num));
}
