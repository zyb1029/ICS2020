#include<stdio.h>
void init_monitor(int, char *[]);
void engine_start();
int is_exit_status_bad();

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  
  init_monitor(argc, argv);

  /* Start engine. */
  engine_start();
 
/*  FILE *fp = fopen("~/ics2020/nemu/tools/gen-expr/.input","r");
  assert(fp != NULL);
  uint32_t result;
  char buff[65536];
  while (fgets(buff, 10000, fp) != NULL){
	int len = strlen(buff);
	buff[len - 1] = 0;
	fscanf(buff,"%u", &result);
	int len_result = 0;
	uint32_t tmp = result;
	while(tmp) {
		len_result++;
		tmp/=10;
	}
	bool p = true;
	if(result == expr(buff + len_result, p)){
		puts("OK");
	}
	else {
		printf("%u %s\n", result, buff + len_result);
	}
	fclose(fp);
  }*/
  return is_exit_status_bad();
}
