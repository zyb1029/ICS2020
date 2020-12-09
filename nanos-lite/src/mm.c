#include <memory.h>

static void *pf = NULL;

void* new_page(size_t nr_page) {
  for (int i = 0; i < nr_page; i++) {
	pf = (char *)pf + PGSIZE;
  }
  return pf;
}

static void* pg_alloc(int n) {
  return NULL;
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);
  return;
  vme_init(pg_alloc, free_page);
}
