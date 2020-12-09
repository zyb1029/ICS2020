#include <common.h>
#include "syscall.h"
#include <proc.h>

static bool ex_flag = false;

void do_syscall(Context *c) {
  uintptr_t a[4];
  char *p;
  timeval *tv;
  a[0] = c->GPR1;
  switch (a[0]) {
	case SYS_exit:
		if(ex_flag == true) {
			context_uload(current, "/bin/menu", NULL, NULL);
			switch_boot_pcb();
			assert(0);
			yield();
		}
		else halt(c->GPR2);
		break;
	case SYS_yield:
		yield();
		c->GPRx = 0;
		break;
	case SYS_open:
		p = (char *) c->GPR2;
		c->GPRx = fs_open(p, c->GPR3, c->GPR4);
		assert(c->GPRx != -1);
		break;
	case SYS_read:
		p = (char *) c->GPR3;
		c->GPRx = fs_read(c->GPR2, p, c->GPR4);
		break;
	case SYS_write:
		if (c->GPR2 == 0) {
			c->GPRx = -1;
		} 
		else if (c -> GPR2 > 2) {
			p = (char *) c->GPR3;
			c->GPRx = fs_write(c->GPR2, p, c->GPR4);
		}
		else {
			p = (char *)c->GPR3;
			for (int i = 0; i < c->GPR4; i++)
				putch(*(p + i));
			c->GPRx = c->GPR4;
		}
		break;
	case SYS_lseek:
		c->GPRx = fs_lseek(c->GPR2, c->GPR3, c->GPR4);
		break;
	case SYS_close:
		c->GPRx = fs_close(c->GPR2);
		break;
	case SYS_brk:
		c->GPRx = 0;
		break;
	case SYS_gettimeofday:
		tv = (timeval *) c->GPR2;
	    uint64_t tep = io_read(AM_TIMER_UPTIME).us;	
		tv->seconds = tep / 1000000;
		tv->useconds = tep - tv->seconds * 1000000;
		c->GPRx = 0;  
		break; 
	case SYS_execve:
		ex_flag = true;
		context_uload(current, (char *)c->GPR2, NULL, NULL);
		switch_boot_pcb();
		c->GPRx = -1;
		yield();
		break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
