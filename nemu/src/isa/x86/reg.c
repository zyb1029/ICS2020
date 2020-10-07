#include <isa.h>
#include <stdlib.h>
#include <time.h>
#include "local-include/reg.h"

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
  srand(time(0));
  word_t sample[8];
  word_t pc_sample = rand();
  cpu.pc = pc_sample;

  int i;
  for (i = R_EAX; i <= R_EDI; i ++) {
    sample[i] = rand();
    reg_l(i) = sample[i];
    assert(reg_w(i) == (sample[i] & 0xffff));
  }

  assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
  assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
  assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
  assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
  assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
  assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
  assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
  assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

  assert(sample[R_EAX] == cpu.eax);
  assert(sample[R_ECX] == cpu.ecx);
  assert(sample[R_EDX] == cpu.edx);
  assert(sample[R_EBX] == cpu.ebx);
  assert(sample[R_ESP] == cpu.esp);
  assert(sample[R_EBP] == cpu.ebp);
  assert(sample[R_ESI] == cpu.esi);
  assert(sample[R_EDI] == cpu.edi);

  assert(pc_sample == cpu.pc);
}

void isa_reg_display() {
	for (int i = 0; i < 8; i++) {
		printf("%-20s0x%-20x%-20d\n",regsl[i], reg_l(i), reg_l(i));
	}
	for (int i = 0; i < 8; i++) {
		printf("%-20s0x%-20x%-20d\n",regsw[i], reg_w(i), reg_w(i));
	}
	for (int i = 0; i < 8; i++) {
		printf("%-20s0x%-20x%-20d\n",regsb[i], reg_b(i), reg_b(i));
	}
}

word_t isa_reg_str2val(const char *s, bool *success) {
	char t[32];
	strcpy(t,s);
	int i;
	int len = strlen(t);
	for (i = 0; i < len; i++) {
		if(t[i] < 'Z') {
			t[i] += 'a' - 'A';
		}
	}
	*success = false;
	word_t ans = 0;
	for (i = 0; i < 8; i++) {
		if (strcmp(t, regsl[i]) == 0) {
			ans = reg_l(i); 
			*success = true;
		}
		else if (strcmp(t, regsw[i]) == 0) {
			ans = reg_w(i);
			*success = true;
		}
		else if (strcmp(t, regsb[i]) == 0) {
			ans = reg_b(i);
			*success = true;
		}
		else if (strcmp(t, "pc") == 0) {
			ans = cpu.pc;
			*success = true;	
		}
	}
  if (strcmp(t, "cf") == 0) {
	  ans = cpu.eflags.CF;
	  *success = true;
  }
  if (strcmp(t, "of") == 0) {
	  ans = cpu.eflags.OF;
	  *success = true;
  }
  if (strcmp(t, "zf") == 0) {
	  ans = cpu.eflags.ZF;
	  *success = true;
  }
  if (strcmp(t, "sf") == 0) {
	  ans = cpu.eflags.SF;
	  *success = true;
  }
  return ans;
}













