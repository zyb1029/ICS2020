#include <proc.h>
#include <common.h>
#include <memory.h>

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

	uintptr_t *loc;
	loc = ((uintptr_t *)new_page(4) - 1);
	printf("%p\n", loc);
    int env_argc = 0;
	for (int i = 0; ; i++)
		if (envp[env_argc] == NULL) break;
		else env_argc++;
	for (int i = env_argc; i >= 0; i--){
		*loc = (uintptr_t)envp[i];
		loc = loc - 1;	
	}

    int argc = 0;
	for (int i = 0; ; i++)
		if (argv[argc] == NULL) break;
		else argc++;
	for (int i = argc; i >= 0; i--){
		*loc = (uintptr_t)argv[i];
		loc = loc - 1;	
	}
	*loc = (uintptr_t)argc;
	Area area;
	area.end = (void *)loc;
	pcb -> cp = ucontext(&(pcb->as), area,(void *)loader(NULL, filename));
	pcb -> cp -> GPRx = (uintptr_t)loc;
	/*
	for (int i = 0; ;i++)
		if (argv[argc[tot]] != NULL) argc[tot]++;
		else break;
	p[tot][0] = (uintptr_t )(&argc[tot]);
	p[tot][1] = (uintptr_t )(argv);
	p[tot][2] = (uintptr_t )(envp);return;
	pcb -> cp -> GPRx = (uintptr_t)p[tot];
	tot++;
	*/ 
}


static char *argv[] = {"--skip", "a", NULL};
static char *envp[] = {"PATH=chy", NULL};

void init_proc() {

  //context_kload(&pcb[0], (void *)hello_fun, (void *)"-bb");
  context_uload(&pcb[0], "/bin/menu", argv, envp);
  switch_boot_pcb();;
 /* 
  Log("Initializing processes...");
  char *pathname = "/bin/menu";
  naive_uload(NULL, pathname);
  */
  // load program here

}

Context* schedule(Context *prev) {assert(0);
  current -> cp = prev;
  current = (current == &pcb[0] ? &pcb[0] : &pcb[0]);
  return current -> cp;
}
