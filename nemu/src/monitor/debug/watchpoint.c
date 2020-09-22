#include "watchpoint.h"
#include "expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
void new_wp(char *args) {
	if (free_ == NULL) {
		assert(0);
	}
	else {
		bool expr_state = true;
		uint32_t expr_val = expr(args, &expr_state);
		if (expr_state == false) {
			if (expr_val == 1) {
				puts("Illegal expression!");
			}
			else {
				puts("Divide by zero!");
			}
			return;
		}
		WP* tmp = free_;
		tmp -> next = NULL;
		free_ = free_ -> next;
		strcpy(tmp->str, args);
		tmp -> val = expr_val;
		if (head != NULL) {
			tmp -> next = head;	
			head = tmp;
		}
		else {
			head = tmp;
		}
		return;
	}
}

void free_wp(WP *wp) {
	if (free_ == NULL) {
		free_ = wp;
	}
	else {
		WP* tmp = free_;
		while(tmp -> next != NULL) {
			tmp = tmp -> next;
		}
		tmp -> next = wp;
	}

}
