#include <SDL.h>
#include <SDL_bmp.h>
#include <stdio.h>
#include <assert.h>

#define W 400
#define H 300

// USAGE:
//   j/down - page down
//   k/up - page up
//   gg - first page

// number of slides
const int N = 53;
// slides path pattern (starts from 0)
const char *path = "/share/slides/slides-%d.bmp";

static SDL_Surface *slide = NULL;
static int cur = 0;

void render() {
  if (slide != NULL) {
    SDL_FreeSurface(slide);
  }
  char fname[256];
  sprintf(fname, path, cur);
  slide = SDL_LoadBMP(fname);
  assert(slide);
  SDL_UpdateRect(slide, 0, 0, 0, 0);
}

int main() {
  SDL_Init(0);
  SDL_Surface *screen = SDL_SetVideoMode(W, H, 32, SDL_HWSURFACE);

  int rep = 0, g = 0;
  
  render();
  return 0;
}
