#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  int* tep;
  tep = (int *)args[0];
  int argc = *tep;

  char **argv, **envp;

  char *tep1;
  tep1 = (char *) args[1];
  argv = (char **)(*tep1);

  uintptr_t *tep2;
  tep2 = (char *) args[2];
  envp = (char **)(*tep2);

  printf("%d %p %p\n", argc, tep1, tep2);
  environ = envp;
  while(1);
  exit(main(argc, argv, envp));
  assert(0);
}
