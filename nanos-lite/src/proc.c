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

void context_uload(PCB * pcb, const char* filename, char *const argv[], char *const envp[]) {
	Area area;
	area.end = heap.end;
	pcb -> cp = ucontext(NULL, area, (void *)loader(NULL, filename));
	int argc = 0;
	for (int i = 0; ;i++)
		if (argv[argc] != NULL) argc++;
		else break;
	int argc_env = 0;
	for (int i = 0; ; i++)
		if (envp[argc_env] != NULL) argc_env++;
		else break;
	printf("%d\n", argc_env);
	while(1);
	pcb -> cp -> GPRx = (intptr_t)heap.end;
}

void init_proc() {
  context_kload(&pcb[0], (void *)hello_fun, (void *)"-bb");
  char *argv[] = {"--skip", "a", NULL};
  char *envp[] = {NULL};
  context_uload(&pcb[1], "/bin/pal", argv, envp);
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
  return current -> cp;
}
