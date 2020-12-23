#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>
#include <string.h>

char handle_key(SDL_Event *ev);

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}

static char name[256];

static char *argv[15];

static char tep[15][256];

static void sh_handle_cmd(const char *cmd) {
	int len = strlen(cmd), lst = 0, bj = 0, argc = 0;
	char *p = (char *)cmd;
	p[len - 1] = '\0';
	for(int i = 0; i < len;i++) 
		if (p[i] == ' ') {
			p[i] = '\0';
			if (!bj) strcpy(name, p + lst);
			else 
				strcpy(tep[argc], p + lst), argv[argc] = tep[argc], argc++;
			lst = i + 1;
		}
	argv[argc] = NULL;

    if(execvp(name, argv) == -1)
		fprintf(stderr, "\033[31m[ERROR]\033[0m Exec %s failed.\n\n", p);
}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();
  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
          sh_handle_cmd(res);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}
