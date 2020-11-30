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
  event -> key.keysym.sym = 0;
  event -> type = 10;
  char buf[64];
  int val = NDL_PollEvent(buf, sizeof(buf));
  if (val == 0) return 1;
  char tep1[10], tep2[10];
  sscanf(buf, "%s %s", tep1, tep2);
  bool kd_flag = false;
  if (strcmp(tep1, "ku") == 0)event -> type = SDL_KEYUP, kd_flag = true;
  else if(strcmp(tep1, "kd") == 0)event -> type = SDL_KEYDOWN, kd_flag = true;
  else kd_flag = false;
  if (kd_flag){
	for (int i = 0; i < 83; i++)
		if (strcmp(tep2, keyname[i]) == 0) {
			event -> key.keysym.sym = i;
			break;
		}
  }
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
