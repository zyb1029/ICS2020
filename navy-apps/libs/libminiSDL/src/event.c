#include <NDL.h>
#include <SDL.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  char buf[64];
  char tep1[10], tep2[10], tep3[10], tep4[10];
  NDL_PollEvent(buf, sizeof(buf));
  sprintf(buf, "%s %s %s %s", tep1, tep2, tep3, tep4);
  if (strcmp(tep3, "ku"))event -> type = SDL_KEYUP;
  else if(strcmp(tep3, "kd"))event -> type = SDL_KEYDOWN;
//  event -> key.keysym.sym = tep4;
  printf("%d\n", SDLK_L);
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
