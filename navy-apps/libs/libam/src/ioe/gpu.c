#include <am.h>
#include <NDL.h>
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
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if (ctl->pixels != NULL) {
		int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl -> h;
		int _w = W, _h = H;
		NDL_OpenCanvas(&_w, &_h);
		NDL_DrawRect(ctl->pixels, x, y, w,h);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
