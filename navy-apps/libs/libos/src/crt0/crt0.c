#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  char *argv[64];
static char *envp[64];
  int argc = (int)args[0];
  int now = 1;
  for (int i = now, j = 0; ; i++, j++){
	  argv[j] = (char *)args[i];
	  if(argv[j] == NULL)break;
	  else now++;
  }

  now = now + 1;
  for (int i = now, j = 0; ; i++, j++){
	  envp[j] = (char *)args[i];
	  if(envp[j] == NULL)break;
	  else now++;
  }
  environ = envp;
  exit(main(argc, argv, environ));
  assert(0);
}
