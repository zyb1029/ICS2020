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
/*	uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
	for (int i = 0; i < 32 *32; i++)fb[i] = i;
	outl(SYNC_ADDR, 1);*/
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if (ctl->pixels != NULL) {
		int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl -> h;
		uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
		uint32_t * pixels = ctl->pixels;
		int cp_bytes = (w < W - x) ? w : W - x;
		for (int j = 0; j < h && y + j < H; j++) {
			for (int k = 0; k < cp_bytes; k++) {
				fb[(y + j) * W + x + k] = *(pixels + k);
			}
			pixels += w;
		}
	}
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
