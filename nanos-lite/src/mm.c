#include <memory.h>
#include <proc.h>
#include <common.h>

static void *pf = NULL;

void* new_page(size_t nr_page) {
  for (int i = 0; i < nr_page; i++) {
	pf = (char *)pf + PGSIZE;
  }
  return pf;
}

static inline void* pg_alloc(int n) {
  assert(n % PGSIZE == 0);
  void *p = new_page(n/PGSIZE);
  memset(p, 0, n);
  return p;
}

void free_page(void *p) {
  panic("not implement yet");
}

extern char _end;

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {/*
  if (brk >= current->max_brk) {
	uintptr_t now;
	now = (current->max_brk) & 0xfffff000;
	void *p = pg_alloc(PGSIZE);
	map(&(current->as), (void *)now, p, 0);
	now = (now & 0xfffff000) + 0x00001000;
	current->max_brk = now;

	while(brk >= current->max_brk) {
		p = pg_alloc(PGSIZE);
		map(&(current->as), (void *)now, p, 0);
		now = (now & 0xfffff000) + 0x00001000;
		current->max_brk = now;
	}
  }			  */
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);
  #ifdef HAS_VME
  vme_init(pg_alloc, free_page);
  #endif
}
