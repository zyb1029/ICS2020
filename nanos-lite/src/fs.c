#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

int open_offset[65536];

int fs_open(const char *pathname, int flags, int mode) {
	int sz = sizeof(file_table) / sizeof(Finfo);
	for (int i = 0; i < sz; i++) 
		if (strcmp(file_table[i].name, pathname) == 0) {
			open_offset[i] = 0;
			return i;
		}
	return -1;	
}

int fs_close(int fb) {
	return 0;
}

int get_head(int fb) {
	assert(fb != -1);
	return file_table[fb].disk_offset;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
}
