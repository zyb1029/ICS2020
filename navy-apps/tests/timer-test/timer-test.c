#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

int main() {
  struct timeval tv;
  int sec = 1;
  while(1) {		  
     gettimeofday(&tv, NULL);
	 while((tv.tv_sec * 1000000 + tv.tv_usec + 500) / 1000000 < sec){
		gettimeofday(&tv, NULL);	 
	}
	 if (sec == 1) printf("%d second\n", sec);
	 else printf("%d seconds\n", sec);
  }
  return 0;
}
