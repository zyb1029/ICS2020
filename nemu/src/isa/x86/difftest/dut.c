#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  if (cpu.eax != ref_r -> eax) {
	  printf("eax should be %x\n", ref_r ->eax);
	  return false;
  }
  if (cpu.ecx != ref_r -> ecx) {
	  printf("ecx should be %x\n", ref_r ->ecx);
	  return false;
  }
  if (cpu.edx != ref_r -> edx) {
	  printf("edx should be %x\n", ref_r ->edx);
	  return false;
  }
  if (cpu.esp != ref_r -> esp) {
	  printf("esp should be %x\n", ref_r ->esp);
	  return false;
  }
  if (cpu.ebp != ref_r -> ebp) {
	  printf("ebp should be %x\n", ref_r ->ebp);
	  return false;
  }
  if (cpu.esi != ref_r -> esi) {
	  printf("esi should be %x\n", ref_r ->esi);
	  return false;
  }
  if (cpu.edi != ref_r -> edi) {
	  printf("edi should be %x\n", ref_r ->edi);
	  return false;
  }
  if (cpu.ebx != ref_r -> ebx) {
	  printf("ebx should be %x\n", ref_r ->ebx);
	  return false;
  }
  return true;
}

void isa_difftest_attach() {
}
