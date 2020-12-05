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
    Log("Hello World from Nanos-lite with arg '%p' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

void context_kload(PCB * pcb, void* loc, void* arg) {
	Area area;
	area.start = pcb;
	printf("%d\n", sizeof(pcb));
	area.end = pcb + sizeof(pcb);
	pcb -> cp = kcontext(area, loc, arg);
}


void init_proc() {
  context_kload(&pcb[0], (void *)hello_fun, NULL);
  switch_boot_pcb();
//  Log("Initializing processes...");
//   char *pathname = "/bin/nterm";
//   naive_uload(NULL, pathname);
  // load program here

}

Context* schedule(Context *prev) {
  return NULL;
}
