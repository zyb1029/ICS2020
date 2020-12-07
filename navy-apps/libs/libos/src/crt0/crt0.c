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
  argv = (char **)args[1];
  envp = (char **)args[2];
  printf("%d %s %s\n", argc, argv[0], envp[0]);
  environ = envp;
  while(1);
  exit(main(argc, argv, envp));
  assert(0);
}
