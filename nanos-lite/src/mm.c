#include <memory.h>

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

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
  printf("%d\n", brk);
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);
  #ifdef HAS_VME
  vme_init(pg_alloc, free_page);
  #endif
}
