#include <stdio.h>
#include <fixedptc.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

int main() {
  int T = 100;
  while(T--) {
  int p = rand() % 100, q = rand() % 100 + 1;
  fixedpt a = fixedpt_rconst(p/q);
  fixedpt b = fixedpt_fromint(p - q);
  int c = fixedpt_toint(fixedpt_floor(fixedpt_mul(a, b)));
  assert(c == floor((p/q) * (p - q)));
  printf("%d\n", c);
  }
  return 0;
}
