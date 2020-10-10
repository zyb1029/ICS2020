static inline def_EHelper(mov) {
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(mov);
}

static inline def_EHelper(push) {
 // TODO();
  rtl_push(s, ddest);
  print_asm_template1(push);
}

static inline def_EHelper(pop) {
 // TODO();
  rtl_pop(s, ddest);
  print_asm_template1(pop);
}

static inline def_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

static inline def_EHelper(popa) {
  TODO();

  print_asm("popa");
}

static inline def_EHelper(leave) {
  rtl_mv(s, s0, &reg_l(R_EBP));
  rtl_mv(s, &reg_l(R_ESP), s0);
  rtl_pop(s, &reg_l(R_EBP));
  print_asm("leave");
}

static inline def_EHelper(cltd) {
  if (s->isa.is_operand_size_16) {
	  int16_t p = reg_w(R_AX);
	  rtlreg_t q;
	  if (p < 0) {
		  q = 0xffff;
	  }
	  else {
		  q = 0;
	  }
	  rtl_li(s, s0, q);
	  rtl_sr(s, R_DX, s0, 2);
  }
  else {
	  int32_t p = reg_l(R_EAX);
	  rtlreg_t q;
	  if (p < 0) {
		  q = 0xffffffff;
	  }
	  else {
		  q = 0;
	  }
	  rtl_li(s, s0, q);
	  rtl_sr(s, R_EDX, s0, 4);
  }
  print_asm(s->isa.is_operand_size_16 ? "cwtl" : "cltd");
}

static inline def_EHelper(cwtl) {
  if (s->isa.is_operand_size_16) {
  TODO();
  }
  else {
  TODO();
  }

  print_asm(s->isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

static inline def_EHelper(movsx) {
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(s, ddest, dsrc1, id_src1->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(movsx);
}

static inline def_EHelper(movzx) {
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  operand_write(s, id_dest, dsrc1);
  if (id_dest->type == OP_TYPE_REG)
	print_Dop(id_dest->str, OP_STR_SIZE, "%%%s", reg_name(id_dest->reg, id_dest->width)); 
  print_asm_template2(movzx);
}

static inline def_EHelper(lea) {
  rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
  operand_write(s, id_dest, ddest);
  print_asm_template2(lea);
}

