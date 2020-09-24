#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buff
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int len = 0;

void rand_space(){
	  if (rand () % 3 == 0) {
		  int t = rand ()% 5, i;
		  for(i = 1; i <= t; i++) {
			  buf[len++] = ' '; buf[len] = 0;
	 	 }
	   }
}
void gen_num() {
	int op = rand () % 2;
	if (op == 0) {
		int p = rand();
		char tmp[10];
		int cnt = 0;
		if(p == 0) {
			buf[len++] = '0';
		}
		else {
			while(p) {
				tmp[cnt++] = p % 10 + '0';
				p/=10;	
			}
			int i;
			for (i = cnt - 1; i >= 0; i--) {
				buf[len++] = tmp[i];
			}
		}
	}
	else if (op == 1) {
		char op_num[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
		int l = rand () % 9;
		int i;
		buf[len++] = '0';
		buf[len++] = 'x';
		for (i = 0; i < l; i++) {
			buf[len++] = op_num[rand() % 16];	
		}
	}

	buf[len++] = 'u';
	buf[len] = 0;
}
void gen_op(){
	int t= rand () % 4, p = rand() % 10;
	if (p == 3) {
		t = 4;
	}
	else if (p == 4) {
		t = 5;	
	}
	else if(p == 5) {
		t = 6;	
	}
 //	printf("%d",t);
  	switch(t){ 
		case 0 : buf[len] = '+'; buf[++len] = 0; break;	
		case 1 : buf[len] = '*'; buf[++len] = 0; break;	
		case 2 : buf[len] = '-'; buf[++len] = 0; break;	
		case 3 : buf[len] = '/'; buf[++len] = 0; break;	
		case 4 : buf[len] = '='; buf[++len] = '='; buf[++len] = 0; break;
		case 5 : buf[len] = '!'; buf[++len] = '='; buf[++len] = 0; break;
		case 6 : buf[len] = '&'; buf[++len] = '&'; buf[++len] = 0; break;
	} 
} 
static inline void gen_rand_expr() {
  int  t = rand()%4;
  if (len > 65536 / 3) {
	  t = 0;
  }
   switch(t) {
	  case 0: rand_space(); gen_num();break; 
	  case 1: buf[len] = '('; 
			  buf[++ len] = 0; rand_space();
			  gen_rand_expr(); rand_space();
			  buf[len] = ')'; buf[++ len] = 0;
			  break;
	  case 2:
	  case 3: 
			  gen_rand_expr();rand_space(); gen_op(); rand_space(); gen_rand_expr(); break;
  }   
}

void remove_u(char *p) {
	int i;
	for (i = 0; p[i] != 0; i++) {
		if (p[i] == 'u') {
			p[i] = ' ';
		}
	}
}

int main(int argc, char *argv[]) {
  buf[0] = 0;
  int seed = time(0);
  srand(seed);
  int loop = 1;
   if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }  
  int i;
  for (i = 0; i < loop; i ++) {
	len = 0;
    gen_rand_expr(); 
	//printf("%s\n", buf);
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);
    int ret = system("gcc /tmp/.code.c -o /tmp/.expr 2> /home/zyb/Desktop/input2 -Wall -Werror ");
    if (ret != 0) {
		i--;
		continue;
	}

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);
	remove_u(buf);
    printf( "%u %s\n", result, buf);
  }
  return 0;
}
