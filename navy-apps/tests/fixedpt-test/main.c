#include <stdio.h>
#include <fixedptc.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

int main() {
  int a = fixedpt_rconst(5.666);
  int b = fixedpt_rconst(2);
  int c = fixedpt_toint(fixedpt_div(a, b));
  printf("%d %d %d \n", c, fixedpt_toint(fixedpt_ceil(a)),fixedpt_toint(fixedpt_floor(a)));
  return 0;
}
