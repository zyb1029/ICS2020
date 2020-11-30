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
}

int main() {
  SDL_Init(0);
  render();
  return 0;
}
