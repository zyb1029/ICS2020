#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

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

struct MenuItem {
  const char *name, *bin, *arg1;
} items[] = {
  {"NJU Terminal", "/bin/nterm", NULL},
  {"NSlider", "/bin/nslider", NULL},
  {"FCEUX (Super Mario Bros)", "/bin/fceux", "/share/games/nes/mario.nes"},
  {"FCEUX (100 in 1)", "/bin/fceux", "/share/games/nes/100in1.nes"},
  {"Flappy Bird", "/bin/bird", NULL},
  {"PAL - Xian Jian Qi Xia Zhuan", "/bin/pal", NULL},
  {"NPlayer", "/bin/nplayer", NULL},
  {"coremark", "/bin/coremark", NULL},
  {"dhrystone", "/bin/dhrystone", NULL},
  {"typing-game", "/bin/typing-game", NULL},
  {"ONScripter", "/bin/onscripter", NULL},
};

static void sh_handle_cmd(const char *cmd) {
	printf("%s/n", cmd);
	for(int i = 0; i < 11; i++)
		if(strcmp(cmd, items[i].bin) == 0)
			execve(items[i].bin, NULL, NULL);
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
