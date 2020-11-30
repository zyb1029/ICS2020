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

SDL_Surface *slide = NULL;

void render() {
  if (slide != NULL) {
	  assert(0);
  }
  char fname[256];
  slide = SDL_LoadBMP(fname);
}

int main() {
  SDL_Init(0);
  render();
  return 0;
}
