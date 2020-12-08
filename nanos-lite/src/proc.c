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

static uintptr_t p[MAX_NR_PROC][3];
static int argc[MAX_NR_PROC];
static int tot = 0;

void context_uload(PCB * pcb, const char* filename, char *const argv[], char *const envp[]) {
	Area area;
	area.end = heap.end - 1;
	pcb -> cp = ucontext(&(pcb->as), area,(void *)loader(NULL, filename));
	pcb -> cp -> GPRx = (uintptr_t)heap.end - 1;
	printf("%p\n", pcb->cp->GPRx);
	return;
	for (int i = 0; ;i++)
		if (argv[argc[tot]] != NULL) argc[tot]++;
		else break;
	p[tot][0] = (uintptr_t )(&argc[tot]);
	p[tot][1] = (uintptr_t )(argv);
	p[tot][2] = (uintptr_t )(envp);
	pcb -> cp -> GPR2 = (uintptr_t)p[tot];
	tot++; 
}


static char *argv[] = {"--skip", "a", NULL};
static char *envp[] = {"PATH=chy"};

void init_proc() {

  //context_kload(&pcb[0], (void *)hello_fun, (void *)"-bb");
  context_uload(&pcb[0], "/bin/pal", argv, envp);
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
  current = (current == &pcb[0] ? &pcb[0] : &pcb[0]);
  return current -> cp;
}
