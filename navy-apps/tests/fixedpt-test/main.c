#include <stdio.h>
#include <fixedptc.h>

int main() {
  fixedpt a = fixedpt_rconst(1.2);
  fixedpt b = fixedpt_fromint(10);
  int c = fixedpt_toint(fixedpt_mul(a, b));
  printf("%d\n", c);
  return 0;
}
