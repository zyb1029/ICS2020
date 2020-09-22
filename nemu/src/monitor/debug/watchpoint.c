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

void watchpoint_check(bool * success) {
	*success = true;	
	WP * tmp = head;
	while (tmp != NULL) {
		bool expr_state = true;
		uint32_t now_expr = expr(tmp -> str,&expr_state);
		if (now_expr != tmp -> val) {
			printf("NO.%d watchpoint changed from %d to %d!",tmp -> NO, tmp -> val, now_expr);
			*success = false;
		}
	}
	return;
}

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
		free_ = free_ -> next;
		tmp -> next = NULL;
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
