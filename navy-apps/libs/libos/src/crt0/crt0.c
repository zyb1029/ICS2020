#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  uintptr_t* tep;
  tep = (uintptr_t *)args[0];
  int argc = (int)(*tep);

  char **argv, **envp;

  uintptr_t *tep1;
  tep1 = (uintptr_t *) args[1];
  argv = (char **)(*tep1);

  uintptr_t *tep2;
  tep2 = (uintptr_t *) args[2];
  envp = (char **)(*tep2);

  printf("%d %s %s\n", argc, argv[0], envp[0]);
  environ = envp;
  while(1);
  exit(main(argc, argv, envp));
  assert(0);
}
