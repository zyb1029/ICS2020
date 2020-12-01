#include <monitor/difftest.h>

static inline def_EHelper(lidt) {
  rtl_lm(s, s0, s->isa.mbase, s->isa.moff, 2);
  cpu.IDTR.size = (int16_t) *ddest;
  rtl_lm(s, s1, s->isa.mbase, s->isa.moff + 2, 4);
  cpu.IDTR.addr = *s1;
  print_asm_template1(lidt);
}

static inline def_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src1->reg, 4), id_dest->reg);
}

static inline def_EHelper(mov_cr2r) {
  TODO();

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
  rtl_pop(s,s0);
  s->jmp_pc = *s0;
  s->is_jmp = true;
  rtl_pop(s,s0);
  cpu.cs = *s0;
  rtl_pop(s,s0);
  cpu.eflags.val = *s0;
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
