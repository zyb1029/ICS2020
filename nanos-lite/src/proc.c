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

static int T = 0;
/*
static char argv2[15][255];
static char envp2[15][255];
*/
void context_uload(PCB * pcb, const char* filename, char *const argv[], char *const envp[]) {
    #ifdef HAS_VME    
	protect(&(pcb->as)); // make copy of directory
    #endif

	uintptr_t *loc;
	loc = ((uintptr_t *)new_page(8) - 1);

    #ifdef HAS_VME
	uintptr_t *loc_tep;
	loc_tep = loc + 1;
	for (int i = 1; i < 8; i++) {
		map(&(pcb->as), (char *)pcb->as.area.end - (i * 0x1000), 
						(char *)loc_tep - (i * 0x1000), 0);
	}
    #endif

	assert(envp != NULL);
    int env_argc = 0;
	if (envp != NULL) {
		for (int i = 0; ; i++)
			if (envp[env_argc] == NULL) break;
			else env_argc++;
		for (int i = env_argc; i >= 0; i--){
			*loc = (uintptr_t)envp[i];
			loc = loc - 1;
		}
	}
	
	assert(argv != NULL);
    int argc = 0;
	if (argv != NULL) {
		for (int i = 0; ; i++)
			if (argv[argc] == NULL) break;
			else argc++;
		for (int i = argc; i >= 0; i--){
			*loc = (uintptr_t)argv[i];
			loc = loc - 1;
		}
	}
	T++;
	*loc = (uintptr_t)argc;
	Area area;
	area.end = (void *)loc;
	printf("%x %x\n", loc, argv[1]);
	if (T == 3)while(1);
    
	#ifdef HAS_VME 
    uint32_t delta = loc_tep - loc;
    uintptr_t *st;
	st = (uintptr_t *)pcb->as.area.end;
	st = st - delta;
    #endif
    
	#ifndef HAS_VME
	pcb -> as.ptr = 0;
	#endif

	pcb -> cp = ucontext(&(pcb->as), area, (void *)loader(pcb, filename));
    
    #ifdef HAS_VME
	pcb -> cp -> GPRx = (uintptr_t)st;
	#endif
	
	#ifndef HAS_VME
	pcb -> cp -> GPRx = (uintptr_t)loc;
	#endif
}


static char *argv[] = {"/bin/exec-test" ,"/share/music/little-star.ogg", NULL};
static char *envp[] = {"PATH/bin/:/usr/bin/", NULL};

void init_proc() {

  context_uload(&pcb[0], "/bin/exec-test", argv, envp);
//  context_uload(&pcb[1], "/bin/hello", argv, envp);
  //context_kload(&pcb[1], (void *)hello_fun, (void *)"-bb");
  switch_boot_pcb();
  
  Log("Initializing processes...");
 /*
  char *pathname = "/bin/menu";
  naive_uload(NULL, pathname);
  */
  // load program here

}
static int count = 0;

Context* schedule(Context *prev) {
  current -> cp = prev;current = &pcb[0]; return current -> cp;
  if (current == &pcb[0]) count = count + 1;
  if (current == &pcb[1]) current = &pcb[0];
  if (count == 500)  count = 0, current = &pcb[1];	  
  else current =  &pcb[0];
  return current -> cp;
}
