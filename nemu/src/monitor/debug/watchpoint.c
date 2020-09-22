#include "watchpoint.h"
#include "expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void wp_display(){
	printf("%-15s%-15s%-15s\n","Num", "Type", "What");
	WP *tmp = head;
	while(tmp != NULL) {
		printf("%-15d%-15s%-15s\n",tmp -> NO, "watchpoint", tmp -> str);
		tmp = tmp -> next;
	}	
	return;	
}

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
			printf("NO.%d watchpoint changed from %d to %d!\n",tmp -> NO, tmp -> val, now_expr);
			*success = false;
		}
		tmp = tmp -> next;
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
			else if(expr_val == 2) {
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
	wp -> next = NULL;
	wp -> val = 0;
	wp -> NO = 0;
	strcpy(wp -> str, "");
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

void del_wp(int wp_num) {
	WP *tmp = head;
	bool del_success = false;
	if (tmp -> NO == wp_num) {
		head = tmp -> next;
		free_wp(tmp);	
		del_success = true;
	}
	else {
		while (tmp -> next != NULL) {
			if (tmp -> next -> NO == wp_num) {
				WP *del_tmp = tmp -> next;
				tmp -> next = tmp -> next -> next;
				free_wp(del_tmp);
				del_success = true;
				break;
			}		
			tmp = tmp -> next;
		}	
	}	
	if (del_success == false) {
		printf("THe No.%d watchpoint doesn't exist\n", wp_num);	
	}
	return;
}
