#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  char *p = (char *)buf;
  for (int i = 0; i < len; i++) {
	  putch(*(p + i));
  }
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  char tep[32];
  bool has_kbd = io_read(AM_INPUT_CONFIG).present;
  if (has_kbd) {
	  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
	  sprintf(tep, "%s %s\n", ev.keydown ? "kd" : "ku", keyname[ev.keycode]);
  }
  int event_len = strlen(tep);
  char *buff = (char *)buf;
  if(event_len > len) event_len = len;
  for (int i = 0; i < event_len; i++) buff[i] = tep[i];
  buff[event_len] = '\0';
  return event_len;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
