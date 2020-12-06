#include <proc.h>
#include <common.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%s' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void context_kload(PCB * pcb, void* loc, void* arg) {
	Area area;
	area.start = pcb;
	area.end = (char *)pcb + sizeof(PCB);
	pcb -> cp = kcontext(area, loc, arg);
}

void context_uload(PCB * pcb, const char* filename) {
	Area area;
	area.end = heap.end;
	pcb -> cp = ucontext(NULL, area, (void *)loader(NULL, filename));
	pcb -> cp -> GPRx = (intptr_t)heap.end;
}

void init_proc() {
  context_kload(&pcb[0], (void *)hello_fun, (void *)"-bb");
 // context_uload(&pcb[1], "/bin/pal");
  switch_boot_pcb();
 /* 
  Log("Initializing processes...");
  char *pathname = "/bin/menu";
  naive_uload(NULL, pathname);
  */
  // load program here

}

Context* schedule(Context *prev) {return prev;
  current -> cp = prev;
  current = (current == &pcb[0] ? &pcb[0] : &pcb[0]);
  return current -> cp;
}
