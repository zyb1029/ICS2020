#include <isa.h>
#include <memory/vaddr.h>
#include <memory/paddr.h>
paddr_t isa_mmu_translate(vaddr_t vaddr, int type, int len) {
   uintptr_t *loc;
   uintptr_t tep = cpu.cr3;
   loc = (uintptr_t *)tep;
   loc = loc + ((vaddr & ~0x3fffff) >> 22);
   assert(loc != NULL);
   printf("%lu\n", *loc);
   assert(((*loc) & 0xfff) == 1);
   assert((*loc) != 0);

   uintptr_t *loc_pt;
   loc_pt = (uintptr_t *) ((*loc) & 0xfffff000);
   loc_pt = loc_pt + ((vaddr & 0x003ff000) >> 12);
   assert(loc_pt != NULL);
   assert(((*loc_pt) & 0xfff) == 1);
   assert((*loc_pt) != 0);
   
   uintptr_t pa;
   pa = ((*loc_pt) & 0xfffff000);
   int real_pa =  pa + (vaddr & 0xfff);

   if (((real_pa + len - 1)& 0xfffff000) != pa) return MEM_RET_CROSS_PAGE;
   else return pa + (vaddr & 0xfff);
}
