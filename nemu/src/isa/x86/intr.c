#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr) {
	vaddr_t gdt_addr = cpu.GDTR.addr + cpu.TR;
	rtl_li(s, s1, gdt_addr);
	rtl_lm(s, s0, s1, 0, 4);
	vaddr_t Tss_addr = (((*s0) & 0xffff0000) >> 16);
	rtl_lm(s, s0, s1, 4, 4);
	Tss_addr += (((*s0) & 0x000000ff) << 16);
	Tss_addr += (((*s0))& 0xff000000);
	rtl_li(s, s1, Tss_addr);
	rtl_lm(s, s0, s1, 4, 4);

	vaddr_t ksp = *s0;
	vaddr_t tep = cpu.esp;
	if (ksp != 0) cpu.esp = ksp;

	rtl_lm(s, s0, s1, 8, 4);
	rtl_li(s, s1, *s0);
	rtl_push(s, s1);
		
	rtl_li(s, s0, tep);
	rtl_push(s, s0);
	printf("r: %x \n", *s0);
	rtl_li(s, s0, cpu.eflags.val);
	cpu.eflags.IF = 0;
	rtl_push(s, s0);

	rtl_li(s, s0, (ksp == 0) ? 8 : 3);
	rtl_push(s, s0);
	rtl_li(s, s0, (s -> is_jmp ? s-> jmp_pc : s -> seq_pc));
	rtl_push(s, s0);
	rtl_li(s, s1, ret_addr);
	rtl_lm(s, s0, s1, NO * 8, 4);
	vaddr_t Jpc = (*s0) & 0xffff;
	rtl_lm(s, s0, s1, NO * 8 + 4, 4);
	Jpc += (*s0) & (0xffff << 16);
	rtl_j(s, Jpc);
	rtl_li(s, s0, 0);
	rtl_li(s, s1, Tss_addr);
	rtl_sm(s, s1, 4, s0, 4);
    printf("%x cc\n", cpu.esp);
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

}

#define IRQ_TIMER 32

void query_intr(DecodeExecState *s) {
	if (cpu.INTR && cpu.eflags.IF) {
		cpu.INTR = false;
		raise_intr(s, IRQ_TIMER, cpu.IDTR.addr);
		update_pc(s);
	}
}
