#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  int argc = (int)args[0];
  printf("%d\n", argc);
  char **argv, **envp;
  int now = 1;
  for (int i = now; ; i++){
	  argv[i] = (char *)args[i];
	  if(argv[i] == NULL)break;
	  else now++;
	  printf("%s\n", argv[i]);
  }
  now = now + 1;
printf("\n");
  for (int i = now; ; i++){
	  envp[i] = (char *)args[i];
	  if(argv[i] == NULL)break;
	  else now++;
	  printf("%s\n", envp[i]);
  }

  environ = envp;
  exit(main(argc, argv, envp));
  assert(0);
}
