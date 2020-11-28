#include <stdio.h>
#include <fixedptc.h>

int main() {
  fixedpt a = fixedpt_rconst(21.5);
  fixedpt b = fixedpt_fromint(10);
  int c = fixedpt_toint(fixedpt_floor(fixedpt_add(a, b)));
  printf("%d\n", c);
  return 0;
}
