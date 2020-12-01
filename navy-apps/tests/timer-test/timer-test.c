#include <unistd.h>
#include <stdio.h>
#include <NDL.h>
#include <assert.h>
int *a = NULL;
int p = 3000;
int main() {
  if (a!=NULL)assert(0);
  else {
	a = &p;  
  }
  
  /*
  int sec = 1;
  NDL_Init(0);
  while(1) {		  
	 while(NDL_GetTicks() / 1000000 < sec);
	 if (sec == 1) printf("%d second\n", sec);
	 else printf("%d seconds\n", sec);
	 sec++;
  }*/
  return 0;
}
