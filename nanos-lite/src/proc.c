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


void init_proc() {
  context_kload(&pcb[0], (void *)hello_fun, (void *)"-bb");
  context_kload(&pcb[1], (void *)hello_fun, (void *)"-aa");
  switch_boot_pcb();
 /* 
  Log("Initializing processes...");
  char *pathname = "/bin/menu";
  naive_uload(NULL, pathname);
  */
  // load program here

}

Context* schedule(Context *prev) {
  current -> cp = prev;
  current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  printf("%p %p\n", (intptr_t) current, &(current->as.ptr));
  return current -> cp;
}
