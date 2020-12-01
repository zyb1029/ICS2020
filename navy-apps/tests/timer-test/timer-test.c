#include <unistd.h>
#include <stdio.h>
#include <NDL.h>
#include <assert.h>
int main() {
  int sec = 1;
  NDL_Init(0);
  while(1) {		  
	 while(NDL_GetTicks() / 1000000 < sec);
	 if (sec == 1) printf("%d second\n", sec);
	 else printf("%d seconds\n", sec);
	 sec++;
  }
  return 0;
}
