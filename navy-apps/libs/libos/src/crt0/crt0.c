#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  int argc = (int)args[0];
  printf("%d\n", argc);
  while(1);
  char **argv, **envp;

  argv = (char **) args[1];
  envp = (char **) args[2];
  environ = envp;
  char **empty = {NULL};
  environ = empty;
  exit(main(0, empty, empty));
  assert(0);
}
