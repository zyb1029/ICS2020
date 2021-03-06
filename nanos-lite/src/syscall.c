#include <common.h>
#include "syscall.h"
#include <proc.h>
#include <memory.h>

static bool ex_flag = false;

static char *argv[] = {"cat", NULL};
static char *env[] = {"/bin/", NULL};

void do_syscall(Context *c) {
  uintptr_t a[4];
  char *p;
  timeval *tv;
  a[0] = c->GPR1;
  switch (a[0]) {
	case SYS_exit:
		if(ex_flag == true) {
			context_uload(current, "/bin/nterm", argv, env);
			current->max_brk = 0;
			switch_boot_pcb();
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
		if(current->max_brk < c->GPR3) current->max_brk = c->GPR3;
		c->GPRx = mm_brk(c->GPR2);
		break;
	case SYS_gettimeofday:
		tv = (timeval *) c->GPR2;
	    uint64_t tep = io_read(AM_TIMER_UPTIME).us;	
		tv->seconds = tep / 1000000;
		tv->useconds = tep - tv->seconds * 1000000;
		c->GPRx = 0;  
		break; 
	case SYS_execve:
		if (fs_open((char *)c->GPR2, 0, 0)== -1) {
			c->GPRx = -2;
		}
		else {
			ex_flag = true;
			context_uload(current, (char *)c->GPR2,(char **)c->GPR3, (char **)c->GPR4);
			current->max_brk = 0;
			switch_boot_pcb();
			yield();
		}
		break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
