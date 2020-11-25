#ifndef __COMMON_H__
#define __COMMON_H__
#include <stddef.h>

size_t ramdisk_read(void *buf, size_t offset, size_t len);

int fs_open(const char *pathname, int flags, int mode);


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
