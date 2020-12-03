#include "cc.h"


static inline def_EHelper(test) {
  rtl_and(s, s0, ddest, dsrc1);
  rtl_set_CF(s, rz);
  rtl_set_OF(s, rz);
  rtl_update_ZFSF(s, s0, s->dest.width);
  print_asm_template2(test);
}

static inline def_EHelper(and) {
  rtl_and(s, s0, ddest, dsrc1);
  operand_write(s, id_dest, s0);
  rtl_set_CF(s, rz);
  rtl_set_OF(s, rz);
  rtl_update_ZFSF(s, ddest, s->dest.width);
  printf("%8x  %8x %8x %8x %d\n", cpu.pc,*ddest, *dsrc1, *s0, cpu.eflags.ZF);
  print_asm_template2(and);
}

static inline def_EHelper(xor) {
  rtl_xor(s, s0, dsrc1, ddest);
  operand_write(s, id_dest, s0);
  rtl_set_CF(s, rz);
  rtl_set_OF(s, rz);
  rtl_update_ZFSF(s, ddest, s->dest.width);
  print_asm_template2(xor);
}

static inline def_EHelper(or) {
  rtl_or(s, s0, dsrc1, ddest);
  operand_write(s, id_dest, s0);
  rtl_set_CF(s, rz);
  rtl_set_OF(s, rz);
  rtl_update_ZFSF(s, ddest, s->dest.width);
  print_asm_template2(or);
}

static inline def_EHelper(bsr) {
  uint32_t val = *dsrc1;
  if (val == 0) {
	  rtl_li(s, s0, 1);
	  rtl_set_ZF(s, s0);
  }
  else {
	rtl_set_ZF(s, rz);
	int tmp = s->isa.is_operand_size_16 ? 15 : 31;
	while ((val & (1u << tmp)) == 0) {
		tmp = tmp - 1;
		rtl_li(s, s1, tmp);		
		operand_write(s, id_dest, s1);
	}
  }
  print_asm_template2(bsr);
}
static inline def_EHelper(sar) {
  rtl_sext(s, s1, ddest, id_dest -> width);
  rtl_sar(s, s0, s1, dsrc1);
  rtl_li(s, s2, 1);
  if (*s0 == 0) rtl_set_ZF(s, s2);
  else rtl_set_ZF(s, rz);
  rtl_mv(s, ddest, s0);
  operand_write(s, id_dest, ddest);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}
static inline def_EHelper(ror) {
  uint32_t tmp = *dsrc1;
  uint32_t val = *ddest;
  uint32_t bias = s->isa.is_operand_size_16 ? 15 : 31;
  tmp = tmp % (bias + 1);
  while(tmp--){
	  uint32_t now = val & 1;
	  val = (val >> 1) + (now << bias); 
  }
  rtl_li(s, s0, val);
  operand_write(s, id_dest, s0);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(ror);
}



static inline def_EHelper(rol) {
  uint32_t tmp = *dsrc1;
  uint32_t val = *ddest;
  uint32_t bias = s->isa.is_operand_size_16 ? 15 : 31;
  tmp = tmp % (bias + 1);
  while(tmp--){
	  uint32_t now = (val & (1 << bias)) >> bias;
	  val = (val << 1) + now; 
  }
  rtl_li(s, s0, val);
  operand_write(s, id_dest, s0);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(rol);
}

static inline def_EHelper(shl) {
  //TODO();
  rtl_shl(s, s0, ddest, dsrc1);
  rtl_mv(s, ddest, s0);
  rtl_li(s, s1, 1);
  if (*s0 == 0) rtl_set_ZF(s, s1);
  else rtl_set_ZF(s, rz);
  operand_write(s, id_dest, ddest);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

static inline def_EHelper(shrd) {
  //TODO();
  uint32_t ShiftAmt = *dsrc1 % 32;
  uint32_t size = (s->isa.is_operand_size_16) ? 16 : 32;
  assert(size > ShiftAmt);
  rtl_li(s, s1, ShiftAmt);
  rtl_shr(s, s0, ddest, s1);
  rtl_li(s, s1, size - ShiftAmt);
  rtl_shl(s, s2, dsrc2, s1);
  rtl_or(s, ddest, s1, s2);
  operand_write(s, id_dest, ddest);
  rtl_update_ZFSF(s, ddest, id_dest -> width);
  rtl_set_OF(s, rz);
  // unnecessary to update CF and OF in NEMU

  print_asm_template3(shld);
}

static inline def_EHelper(shld) {
  //TODO();
  uint32_t ShiftAmt = *dsrc1 % 32;
  uint32_t size = (s->isa.is_operand_size_16) ? 16 : 32;
  assert(size > ShiftAmt);
  rtl_li(s, s1, ShiftAmt);
  rtl_shl(s, s0, ddest, s1);
  rtl_li(s, s1, size - ShiftAmt);
  rtl_shr(s, s2, dsrc2, s1);
  rtl_or(s, ddest, s1, s2);
  operand_write(s, id_dest, ddest);
  rtl_update_ZFSF(s, ddest, id_dest -> width);
  rtl_set_OF(s, rz);
  print_asm_template3(shld);
}

static inline def_EHelper(shr) {
  rtl_shr(s, s0, ddest, dsrc1);
  rtl_li(s, s1, 1);
  if (*s0 == 0) rtl_set_ZF(s, s1);
  else rtl_set_ZF(s, rz);
  rtl_mv(s, ddest, s0);
  operand_write(s, id_dest, ddest);
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(shr);
}


static inline def_EHelper(setcc) {
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, ddest, cc);
  operand_write(s, id_dest, ddest);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

static inline def_EHelper(not) {
  rtl_li(s, s0, 0xffffffffu);
  rtl_xor(s, s1, ddest, s0);
  operand_write(s, id_dest, s1);
  print_asm_template1(not);
}

