#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

#define W 400
#define H 300

void __am_gpu_init() {
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = W,  .height = H,
    .vmemsz = 0
  };
  /*uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (int i = 0; i < 400 * 300; i++)fb[i] = i;
  outl(SYNC_ADDR, 0); */
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
  if (ctl->pixels != NULL) {
		int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl -> h;
		uint32_t * pixel = ctl->pixels;
		int p = (w < W - x) ? w : W - x;
		for (int j = 0; j < h && y + j < H; j++) {
			for (int k = x; k < x + p; k++) {
				outl(FB_ADDR /*+ ((y + j) * W + k)*/ , *(pixel + k));
			}
			pixel += w;
		}
	}
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
