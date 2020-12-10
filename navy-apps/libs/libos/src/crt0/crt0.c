#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  int argc = (int)args[0];
  char *argv[64];
  char *envp[64];
  int now = 1;
  // printf("%p\n", args);
  for (int i = now, j = 0; ; i++, j++){
	  argv[j] = (char *)args[i];
	  if(argv[j] == NULL)break;
	  else now++;
	 // printf("%s\n", argv[j]);
  }
  now = now + 1;
  for (int i = now, j = 0; ; i++, j++){
	  envp[j] = (char *)args[i];
	  if(envp[j] == NULL)break;
	  else now++;
	  // printf("%s\n", envp[j]);
  }
  if (strcmp(envp[0], "PATH=/bin/:/usr/bin/")!= 0)while(1);
  environ = envp;
  exit(main(argc, argv, envp));
  assert(0);
}
