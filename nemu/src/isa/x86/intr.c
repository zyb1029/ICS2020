#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr) {
	rtl_li(s, s0, cpu.pc);
	rtl_push(s, s0);
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  TODO();
}

void query_intr(DecodeExecState *s) {
  TODO();
}
