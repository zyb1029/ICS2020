#include <am.h>
#include <nemu.h>
#include <klib.h>

static AddrSpace kas = {};
static void* (*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static Area segments[] = {      // Kernel memory mappings
  NEMU_PADDR_SPACE
};

#define USER_SPACE RANGE(0x40000000, 0xc0000000)

bool vme_init(void* (*pgalloc_f)(int), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  kas.ptr = pgalloc_f(PGSIZE);
  int i;
  for (i = 0; i < LENGTH(segments); i ++) {
    void *va = segments[i].start;
    for (; va < segments[i].end; va += PGSIZE) {
      map(&kas, va, va, 0); 
    }
  }
  set_cr3(kas.ptr);
  set_cr0(get_cr0() | CR0_PG);
  vme_enable = 1;
  return true;
}

void protect(AddrSpace *as) {
  PTE *updir = (PTE*)(pgalloc_usr(PGSIZE));
  as->ptr = updir;
  as->area = USER_SPACE;
  as->pgsize = PGSIZE;
  // map kernel space
  memcpy(updir, kas.ptr, PGSIZE);
}

void unprotect(AddrSpace *as) {
}

void __am_get_cur_as(Context *c) {
  c->cr3 = (vme_enable ? (void *)get_cr3() : NULL);
}

void __am_switch(Context *c) {
  if (vme_enable && c->cr3 != NULL) {
    set_cr3(c->cr3);
	printf("%x\n", c->cr3);
  }
}

void map(AddrSpace *as, void *va, void *pa, int prot) {
	assert(as != NULL);
	uintptr_t *loc;
	loc = (uintptr_t *)as->ptr;
	if ((uintptr_t)loc % PGSIZE != 0) {
		printf("%p\n", loc);
	}
	assert((uintptr_t)loc % PGSIZE == 0);
    
	uintptr_t src = (uintptr_t)va;
	assert(src % PGSIZE == 0);
	uintptr_t dst = (uintptr_t)pa;
	assert(dst % PGSIZE == 0);
	
	loc = loc + ((src & ~0x3fffff) >> 22); // location in directry
	uintptr_t *loc_pt; // page table's location
	if (*loc == 0) {
		uintptr_t *tep;
		tep = (uintptr_t *)pgalloc_usr(PGSIZE);
		assert(((uintptr_t)tep & 0xfff) == 0);
		*loc = (((uintptr_t)tep) | 1);
		loc_pt = tep;
	}
	else loc_pt = (uintptr_t *) ((*loc) & 0xfffff000);
	loc_pt = loc_pt + ((src & 0x003ff000) >> 12);
	if (*loc_pt != 0) {
		if (*loc_pt != (dst | 1)) printf("repeat mapping %x\n", src);
		//assert(*loc_pt == (dst | 1));
	}
	else
	 *loc_pt = (dst | 1);
}

Context* ucontext(AddrSpace *as, Area kstack, void *entry) {
  Context *c = (Context *)kstack.end - 1;
  c->cr3 = (void *)as->ptr;
  c->edi = 0, c->esi = 0, c->ebp = 0, c->esp = 0, c->ebx = 0, c->edx = 0;
  c->ecx = 0, c->eax = 0;
  c->irq = 0x81;
  c->esp = (uintptr_t)kstack.end;
//  c->esp  = (uintptr_t)((uintptr_t *)heap.end - 1);
  c->eip = (uintptr_t)(entry); c->cs = 8; c->eflags = 0;
  return c;
}
