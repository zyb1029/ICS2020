#include <am.h>
#include <string.h>
#include <SDL.h>

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
//  uint32_t k = inl(KBD_ADDR);
//  kbd->keydown = (k & KEYDOWN_MASK ? true : false);
//  kbd->keycode = k & ~KEYDOWN_MASK;
	SDL_Event  ev;
	ev.key.keysym.sym = 0;
	SDL_PollEvent(&ev);
	kbd->keydown = (ev.type == SDL_KEYUP ? true : false);
	kbd->keycode = ev.key.keysym.sym;
}
