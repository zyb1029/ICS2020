#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  printf("%p\n", args[2]);
  uintptr_t argc;
  argc = (uintptr_t)args[0]; 
  printf("%p\n", argc);

  while(1);	
  char *empty[] =  {NULL };
  environ = empty;
  exit(main(0, empty, empty));
  assert(0);
}
