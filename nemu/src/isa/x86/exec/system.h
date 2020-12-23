#include <monitor/difftest.h>

static inline def_EHelper(ltr) {
  rtl_li(s, s1, 0);
  rtl_add(s, s0, s1, ddest);
  cpu.TR = (uint16_t)*s0;
  print_asm_template1(ltr);
}

static inline def_EHelper(lidt) {
  rtl_lm(s, s0, s->isa.mbase, s->isa.moff, 2);
  cpu.IDTR.size = (int16_t) *s0;
  rtl_lm(s, s1, s->isa.mbase, s->isa.moff + 2, 4);
  cpu.IDTR.addr = *s1;
  print_asm_template1(lidt);
}

static inline def_EHelper(lgdt) {
  rtl_lm(s, s0, s->isa.mbase, s->isa.moff, 2);
  cpu.GDTR.size = (int16_t) *s0;
  rtl_lm(s, s1, s->isa.mbase, s->isa.moff + 2, 4);
  cpu.GDTR.addr = *s1;
  print_asm_template1(lgdt);
}

static inline def_EHelper(mov_r2cr) {
  assert(id_dest->reg == 3 || id_dest->reg == 0);
  assert(s->src1.width == 4);
  rtl_lr(s, s0, id_src1->reg, 4);
  if (id_dest ->reg == 3) cpu.cr3 = (uint32_t)*s0;
  else cpu.cr0 = (uint32_t)*s0;
  print_asm("movl %%%s,%%cr%d", reg_name(id_src1->reg, 4), id_dest->reg);
}

static inline def_EHelper(mov_cr2r) {
  assert(id_src1->reg == 0 || id_src1-> reg == 3);
  assert(s->dest.width == 4);
  if (id_src1->reg == 0)rtl_li(s, s0, cpu.cr0);
  else rtl_li(s, s0, cpu.cr3);
  rtl_sr(s, id_dest->reg, s0, 4);

  print_asm("movl %%cr%d,%%%s", id_src1->reg, reg_name(id_dest->reg, 4));

#ifndef __DIFF_REF_NEMU__
  difftest_skip_ref();
#endif
}
static inline def_EHelper(movsb) {
  if (s->isa.is_operand_size_16) {
	TODO();   
  }
  else {
	rtl_lm(s, s0, &reg_l(R_ESI), 0, 1);
	rtl_sm(s, &reg_l(R_EDI), 0, s0, 1);
	rtl_addi(s, &reg_l(R_EDI), &reg_l(R_EDI), 1);	  
	rtl_addi(s, &reg_l(R_ESI), &reg_l(R_ESI), 1);	  
   }
//  print_asm("movl %%cr%d,%%%s", id_src1->reg, reg_name(id_dest->reg, 4));

}

static inline def_EHelper(movsl) {
  if (s->isa.is_operand_size_16) {
	TODO();   
  }
  else {
	rtl_lm(s, s0, &reg_l(R_ESI), 0, 4);
	rtl_sm(s, &reg_l(R_EDI), 0, s0, 4);
	rtl_addi(s, &reg_l(R_EDI), &reg_l(R_EDI), 4);	  
	rtl_addi(s, &reg_l(R_ESI), &reg_l(R_ESI), 4);	  
   }
}

static inline def_EHelper(int) { 
  raise_intr(s, *ddest, cpu.IDTR.addr);
  print_asm("int %s", id_dest->str);

#ifndef __DIFF_REF_NEMU__
  difftest_skip_dut(1, 2);
#endif
}

static inline def_EHelper(iret) {
  rtl_pop(s, s0);
  s->jmp_pc = *s0;
  s->is_jmp = true;
  rtl_pop(s, s0);
  cpu.cs = *s0;

  rtl_pop(s, s0);
  cpu.eflags.val = *s0;

	 vaddr_t gdt_addr = cpu.GDTR.addr + cpu.TR;
	 rtl_li(s, s1, gdt_addr);
	 rtl_lm(s, s0, s1, 0, 4);
	 vaddr_t Tss_addr = (((*s0) & 0xffff0000) >> 16);
	 rtl_lm(s, s0, s1, 4, 4);
	 Tss_addr += (((*s0) &0x000000ff) << 16);
	 Tss_addr += ((*s0) & 0xff000000);
	 rtl_pop(s, s0);
	 vaddr_t tep = *s0;
	 rtl_pop(s, s0);
	 rtl_li(s, s1, Tss_addr);
	 rtl_sm(s, s1, 8, s0, 4);

	 rtl_li(s, s1, Tss_addr);
	 rtl_li(s, s0, tep);
	 if ((cpu.cs & 0x3) == 0x3)
		  rtl_sm(s, s1, 4, s0, 4), printf("iret: %x\n", cpu.esp);
	 if (tep) cpu.esp = tep;
  print_asm("iret");

#ifndef __DIFF_REF_NEMU__
  difftest_skip_ref();
#endif
}

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);

static inline def_EHelper(in) {
   switch (id_dest -> width) {
	case 1:	  
	  rtl_li(s, s0, pio_read_b(*dsrc1));
	  rtl_sr(s, R_EAX, s0, 1);
	  break;
	case 4:	  
	  rtl_li(s, s0, pio_read_l(*dsrc1));
	  rtl_sr(s, R_EAX, s0, 4);
	  break;
	default: TODO();  
  }
  print_asm_template2(in);
}

static inline def_EHelper(out) {
  switch (id_dest -> width) {
	case 1:	  
	  pio_write_b(*ddest, *dsrc1);
	  break;
	case 4:
	  pio_write_l(*ddest, *dsrc1);
	  break;
	default: TODO();  
  }
  print_asm_template2(out);
}
