static inline def_EHelper(mov) {
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(mov);
}

static inline def_EHelper(push) {
 // TODO();
  if (s->isa.is_operand_size_16) assert(0);
  rtl_push(s, ddest);
  print_asm_template1(push);
}

static inline def_EHelper(pop) {
 // TODO();
  if (s->isa.is_operand_size_16) assert(0);
  rtl_pop(s, ddest);
  print_asm_template1(pop);
}

static inline def_EHelper(pusha) {
  int size = 4;
  if (s->isa.is_operand_size_16) assert(0);
  rtl_lr(s, s0, R_ESP, size);
  rtl_lr(s, s1, R_EAX, size);
  rtl_push(s, s1);
  rtl_lr(s, s1, R_ECX, size);
  rtl_push(s, s1);
  rtl_lr(s, s1, R_EDX, size);
  rtl_push(s, s1);
  rtl_lr(s, s1, R_EBX, size);
  rtl_push(s, s1);
  rtl_push(s, s0);
  rtl_lr(s, s1, R_EBP, size);
  rtl_push(s, s1);
  rtl_lr(s, s1, R_ESI, size);
  rtl_push(s, s1);
  rtl_lr(s, s1, R_EDI, size);
  rtl_push(s, s1);
  print_asm("pusha");
}

static inline def_EHelper(popa) {
  if (s->isa.is_operand_size_16) assert(0);
  int size = 4;
  rtl_pop(s, s0);
  rtl_sr(s, R_EDI, s0 ,size);
  rtl_pop(s, s0);
  rtl_sr(s, R_ESI, s0 ,size);
  rtl_pop(s, s0);
  rtl_sr(s, R_EBP, s0 ,size);
  rtl_pop(s, s0);
  rtl_pop(s, s0);
  rtl_sr(s, R_EBX, s0 ,size);
  rtl_pop(s, s0);
  rtl_sr(s, R_EDX, s0 ,size);
  rtl_pop(s, s0);
  rtl_sr(s, R_ECX, s0 ,size);
  rtl_pop(s, s0);
  rtl_sr(s, R_EAX, s0 ,size);
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
	 rtl_lr(s, s1, R_EAX, 1);
     rtl_sext(s, s0, s1, 1);
	 rtl_sr(s, R_EAX, s0, 2);
  }
  else {
	 rtl_lr(s, s1, R_EAX, 2);
     rtl_sext(s, s0, s1, 2);
	 rtl_sr(s, R_EAX, s0, 4);
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

