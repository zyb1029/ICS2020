#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  uintptr_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  int irq;
  void *cr3;
  uintptr_t eflags, cs, eip;	
};

#define GPR1 eax
#define GPR2 eip
#define GPR3 eip
#define GPR4 eip
#define GPRx eip

#endif
