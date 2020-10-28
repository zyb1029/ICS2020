#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr) {
	rtl_li(s, s0, cpu.eflags.val);
	rtl_push(s, s0);
	rtl_li(s, s0, cpu.cs);
	rtl_push(s, s0);
	rtl_li(s, s0, cpu.pc);
	rtl_push(s, s0);
	rtl_li(s, s1, ret_addr);
	rtl_lm(s, s0, s1, NO * 8, 4);
	vaddr_t Jpc = (*s0) >> 16;
	rtl_lm(s, s0, s1, NO * 8 + 4, 4);
	Jpc += (*s0) << 16;
	printf("%08x", Jpc);
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  TODO();
}

void query_intr(DecodeExecState *s) {
  TODO();
}
