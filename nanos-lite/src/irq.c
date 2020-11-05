#include <common.h>

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
	case EVENT_YIELD: printf("touch yield!\n"); break;
	case EVENT_SYSCALL: printf("touch syscall!\n"); break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
