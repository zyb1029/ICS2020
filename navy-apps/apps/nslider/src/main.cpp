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

int *a = NULL;
void render() {
  if (a != NULL) {
	  assert(0);
  }
  else {
	  int p;
	  a = &p;
  }
}

int main() {
  SDL_Init(0);
  render();
  return 0;
}
