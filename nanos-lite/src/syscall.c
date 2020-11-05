#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  printf("%d", a[0]);
  switch (a[0]) {
	case SYS_exit:
		halt(SYS_exit);
		break;
	case SYS_yield:
		yield();
		break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
