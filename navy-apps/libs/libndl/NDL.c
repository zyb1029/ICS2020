#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int canvas_w = 0, canvas_h = 0;

uint32_t NDL_GetTicks() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000000 + tv.tv_usec;
}

int NDL_PollEvent(char *buf, int len) {
  int fd = open("/dev/events", O_RDONLY);
  int val = read(fd, buf, len);
  close(fd);
  val = val ? 1 : 0; 
  return val;
}

void NDL_OpenCanvas(int *w, int *h) {
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h; // ??????
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }
  else {
	  char buf[32], tep1[10], tep2[10];
	  read(evtdev, buf, 32);
	  sscanf(buf, "%s %d %s %d", tep1, &screen_w, tep2, &screen_h);
	  if (*w == 0 && *h == 0) *w = screen_w, *h = screen_h;
	  canvas_w = *w;
	  canvas_h = *h;
   }
}
// (x,y)   w * h

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
	x = ((screen_w - canvas_w) >> 1) + x;
	y = ((screen_h - canvas_h) >> 1) + y;
	for (int i = 0; i < h; i++) {
		lseek(fbdev, ((y + i) * screen_w + x) << 2, SEEK_SET);
		write(fbdev, pixels + i * w, w << 2);
	}
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  fbdev = open("/dev/fb", O_WRONLY);
  evtdev = open("/dev/events", O_RDONLY);
  return 0;
}

void NDL_Quit() {
	if (fbdev != -1)close(fbdev);	
	if (evtdev != -1)close(evtdev);
}
