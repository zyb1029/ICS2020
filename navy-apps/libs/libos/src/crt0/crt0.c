#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  printf("%p\n", args);
  int* argc;
  argc = (uintptr_t )args[0]; 
  printf("%d\n", *argc);

  while(1);	
  char *empty[] =  {NULL };
  environ = empty;
  exit(main(0, empty, empty));
  assert(0);
}
