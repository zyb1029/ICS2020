#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  switch (a[0]) {
	case SYS_exit:
		halt(SYS_exit);
		break;
	case SYS_yield:
		yield();
		c->GPRx = 0;
		break;
	case SYS_write:
		if (c -> GPR2 != 1 && c -> GPR2 != 2){
			c -> GPRx = -1;	
		}
		else {
			char * p = (char *)c->GPR3;
			for (int i = 0; i < c->GPR4; i++)
				putch(*(p + i));
			c->GPRx = c->GPR4;
		}
		break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
