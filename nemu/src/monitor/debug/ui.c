#include <isa.h>
#include "expr.h"
#include "watchpoint.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <memory/paddr.h>

void cpu_exec(uint64_t);
int is_batch_mode();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char buff[65536];
static char* rl_gets() {

  FILE *fp = fopen("/home/zyb/ics2020/nemu/tools/gen-expr/input","r");
 // FILE *fp = fopen("/home/zyb/Downloads/input2","r");
  assert(fp != NULL);
  uint32_t result;
  int t = 0;
  while(fgets(buff, 65536, fp) != NULL) {
	  int len = strlen(buff);
	  if (t < len) {
		  t = len;
	  }
	  printf("%s",buff);
	  buff[len - 1] = 0;
	  sscanf(buff,"%u", &result);
	  int len_result = 0;
	  uint32_t tmp = result;
	  if(tmp == 0) {
		  len_result = 1;
	  }
	  while(tmp) {
		  len_result++;
		  tmp/=10;
	  }
	  bool p =true;
	  printf("%s",buff + len_result);
	  if(result == expr(buff + len_result,&p)) {
		  puts("OK");
		  printf("%s\n%u\n ",buff + len_result, result);
	  }
	  else {
		  assert(0);
		  printf("%u %s\n", result, buff + len_result);
	  }
	  
  }
  fclose(fp);
  printf("the max length is %d\n",t);
  return 0;

/*
   static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
  */
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_x(char *args);

static int cmd_p(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "executive instruction for [n] times", cmd_si },
  { "info", "print all registers", cmd_info },
  { "x", "examine memory", cmd_x },
  { "p", "caculus the expression", cmd_p },
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args) {
	if(args == NULL ){
		cpu_exec(1);
	}

	else {
		char *arg = strtok(NULL, " ");
		int len = strlen(arg);
		for(int i = 0; i < len; i++) {
			if (! (arg[i] <= '9' && arg[i] >= '0' )) {
				printf("si n : n must be nonnegative integer!\n");
				return 0;
			}
		}
		int n = atoi(arg);
		if (n < 0) {
			printf("si [n]: n must be nonnegative integer!\n");
		}
		else {
			cpu_exec(n);
		}
	} 
	return 0;
}
static int cmd_info(char *args) {
	char *arg = strtok(NULL, " ");
	if (strcmp(arg, "r") ==0 ) {
		isa_reg_display();
	}
	return 0;
}

static int cmd_x(char *args) {
	char *arg = strtok(NULL, " ");
	int n = atoi(arg);
	char *now = strtok(NULL," ");
	int num;
	sscanf(now, "%x" , &num);
	int i;
	printf("0x%x:\t",num);
	uint32_t  addr = paddr_read(num, 4);
	for (i = 0; i < n; i++) {
		printf("0x");
		printf("%.8x\t",addr);
		num = num + 4;
		addr = paddr_read(num, 4);
	}
	printf("\n");
	return 0;
}
static int cmd_p(char *args) {
	bool expr_state = true;
	uint32_t expr_val = expr(args, &expr_state);
	if (expr_state == false) {
		if (expr_val == 1) {
			puts("Illegal expression!");
		}
		if (expr_val == 2) {
			puts("Divide by zero!");
		}
	}
	else {
		printf("%u\n", expr_val);
	}
	return 0;
}

void ui_mainloop() {
  if (is_batch_mode()) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
