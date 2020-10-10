#include "cc.h"

static inline def_EHelper(jmp) {
  // the target address is calculated at the decode stage
  rtl_j(s, s->jmp_pc);

  print_asm("jmp %x", s->jmp_pc);
}

static inline def_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, s0, cc);
  rtl_jrelop(s, RELOP_NE, s0, rz, s->jmp_pc);

  print_asm("j%s %x", get_cc_name(cc), s->jmp_pc);
}

static inline def_EHelper(jmp_rm) {
  rtl_jr(s, ddest);

  print_asm("jmp *%s", id_dest->str);
}

static inline def_EHelper(call) {
  // the target address is calculated at the decode stage
  //TODO();
  rtl_j(s, s->jmp_pc);
  rtl_push(s, &s->seq_pc);
  print_asm("call %x", s->jmp_pc);
}

static inline def_EHelper(ret) {
 // TODO();
  rtl_pop(s, &s->jmp_pc);
  s->is_jmp = true;
  print_asm("ret");
}

static inline def_EHelper(xchg) {
	rtl_lr(s, s0, R_EAX, id_dest->width);
	if (id_dest -> type == OP_TYPE_REG) {
		rtl_sr(s, R_EAX, ddest, id_dest -> width);
		rtl_sr(s, id_dest->reg, s0, id_dest -> width);
	}
	else  {
		TODO();

	}
}

static inline def_EHelper(ret_imm) {
  TODO();

  print_asm("ret %s", id_dest->str);
}

static inline def_EHelper(call_rm) {
  rtl_lm(s, s0, ddest, 0, id_dest->width);	
  rtl_jr(s, s0);
  rtl_push(s, &s->seq_pc);

  print_asm("call *%s", id_dest->str);
}
