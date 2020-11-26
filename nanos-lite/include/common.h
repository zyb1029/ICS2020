#ifndef __COMMON_H__
#define __COMMON_H__
#include <stddef.h>

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fb, void *buf, int len);
size_t fs_write(int fb, void *buf, int len);
size_t fs_lseek(int fb, size_t offset, int whence);
int fs_close(int fb);
int get_head(int fb);

size_t serial_write(const void *buf, size_t offset, size_t len);

/* Uncomment these macros to enable corresponding functionality. */
#define HAS_CTE
//#define HAS_VME
//#define MULTIPROGRAM
//#define TIME_SHARING

#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <debug.h>

void do_syscall(Context *c);

#endif
