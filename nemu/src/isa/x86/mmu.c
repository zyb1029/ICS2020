#include <isa.h>
#include <memory/vaddr.h>
#include <memory/paddr.h>
paddr_t isa_mmu_translate(vaddr_t vaddr, int type, int len) {
   uint32_t *loc;
   loc = (uint32_t *)guest_to_host(cpu.cr3);
   loc = loc + ((vaddr & ~0x3fffff) >> 22);
   assert(loc != NULL);
   if (((*loc) & 0xfff) != 1) printf("%x %x %x\n",cpu.cr3, vaddr, *loc);
   assert(((*loc) & 0xfff) == 1);
   assert((*loc) != 0);

   uint32_t *loc_pt;
   loc_pt = (uint32_t *) guest_to_host((*loc) & 0xfffff000);
   loc_pt = loc_pt + ((vaddr & 0x003ff000) >> 12);
   assert(loc_pt != NULL);
   if (((*loc_pt) & 0xfff) != 1)printf("%x %x\n", vaddr, *loc_pt);
   assert(((*loc_pt) & 0xfff) == 1);
   assert((*loc_pt) != 0);
   
   uintptr_t pa;
   pa = ((*loc_pt) & 0xfffff000);
   paddr_t real_pa =  pa + (vaddr & 0xfff);

   if (((real_pa + len - 1)& 0xfffff000) != pa) return MEM_RET_CROSS_PAGE;
   else return pa + (vaddr & 0xfff);
}
