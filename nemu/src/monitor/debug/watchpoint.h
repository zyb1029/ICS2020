#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char str[65536];
  uint32_t val;
  /* TODO: Add more members if necessary */

} WP;


#endif
