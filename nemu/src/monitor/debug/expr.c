#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, STAR, MINUS

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"-", '-'},			// mimus
  {"\\*", '*'},			// mutilply
  {"/",'/'},			// divide
  {"0x[0-9,a-f]+", 'h'},	// %x number
  {"[0-9]+", '0'},		// %d number
  {"\\(", '('},			// left
  {"\\)", ')'},			// right
  {"\\$", '$'},			// reg
  {"[a-zA-Z]+", 'w'},	// reg_name
  {"!=", 'n'},			// not_equal
  {"&&", '&'},			// and
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[65536] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

//        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
//           i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        switch (rules[i].token_type) {
			case TK_NOTYPE : break;
			case '+':
			case '-':
			case '*':
			case '/':
			case '(':
			case ')':
			case '0':
			case 'w':
			case '$':
			case 'n':
			case TK_EQ:
			case '&':
			case 'h':
					 memcpy(tokens[nr_token].str, substr_start,substr_len);
					 tokens[nr_token].type = rules[i].token_type;
				     nr_token = nr_token + 1;	
					 break;
			default: TODO();
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}
static int match_parentheses[65536];

static int st[65536], top = 0;

static bool type1_sign = false;
static bool type2_sign = false;

void pre_check(){
	for (int i = 0; i < nr_token; i++) {
		if(tokens[i].type == '(') {
			st[++top] = i;
		}
		else if(tokens[i].type == ')') {
			if(top == 0) {
				type1_sign = true;
				break;
			}
			else {
				match_parentheses[st[top--]] = i;
			}
		}
	}
	if(top) {
		type1_sign = true;
	}
}

bool check_parentheses(int p,int q){
	if(tokens[p].type != '(' || tokens[q].type != ')') {
		return false;
	}
	else {
		if (match_parentheses[p] == q){
			return true;
		}
		else {
			return false;
		}
	}
}
uint32_t eval(int p, int q) {
	if (type1_sign || type2_sign) {
		return 0;
	}
	if (p > q) {
		type1_sign = true;
		return 0;
	}
	else  if (p == q) { // ?????????????????????
		if(tokens[p].type != '0' && tokens[p].type != 'h') {
			type1_sign = true;
			return 0;
		} 
		else {
			uint32_t tmp;
			if (tokens[p].type == '0') {
				sscanf(tokens[p].str, "%u", &tmp);
			}
			else {
				sscanf(tokens[p].str, "%x", &tmp);
			}
			return tmp;
		} 
	}
	else if (p + 1 == q && tokens[p].type == MINUS  && (tokens[q].type == '0' || tokens[q].type == 'h') ) {    // ????????????
		uint32_t tmp;
		if (tokens[q].type == '0') {
			sscanf(tokens[q].str, "%u", &tmp);
		}
		else if (tokens[q].type == 'h') {
			sscanf(tokens[q].str, "%x", &tmp);
		}
		return -1 * tmp;
	}
	else if (p + 1 == q && tokens[p].type == '$' && tokens[q].type == 'w') { // ???????????????
		bool success = false;
		uint32_t reg_val;
        reg_val = isa_reg_str2val(tokens[q].str, &success);
		if (success == false) {
			type1_sign = true;
			return 0;
		}
		else {
			return reg_val;
		}
	}
		 
	else if (check_parentheses(p, q) == true) {  // ????????????
		return eval(p + 1, q - 1);
	} 
    else {
		int i, sum = 0, op = p, equal_location = 0, unequal_location = 0;
		int and_location = 0;
		int op_type = '0';
		bool equal_sign = false, unequal_sign = false, and_sign = false;
		for (i = q; i > p; i--) {
			if(tokens[i].type == '(') {
				sum--;
	 	 	}
			else if(tokens[i].type == ')') {
				sum++;
	 	 	}
	 		else if(sum == 0){ // ??????????????????????????????????????????????????????????????????
	 			if (tokens[i].type == TK_EQ && equal_sign == false) { //???????????????
					equal_sign = true;
					equal_location = i;
				}
				else if (tokens[i].type == 'n' && unequal_sign == false) {
				//??????????????????
					unequal_sign = true;
					unequal_location = i;
				}
				else if (tokens[i].type == '&') {
			    // ??????and
					and_sign = true;
					and_location = i;
				}
				else if (tokens[i].type == '+' || tokens[i].type == '-' || tokens[i].type == '*' || tokens[i].type == '/'){
					if (op_type == '0') { // ??????????????????????????????
						op_type = tokens[i].type;
						op = i;
	 	 			}
					else if ( (op_type == '*' || op_type == '/') && (tokens[i].type == '+' || tokens[i].type == '-')) { // ??????????????????
						op_type = tokens[i].type;
						op = i;
	 	 			}
		 		}
		 	}
	 	} 
		if (and_sign == true) {
			op = and_location;
			op_type = '&';
		}
		else if (equal_sign == true) {
			op = equal_location;
			op_type = TK_EQ;
		}
		else if (unequal_sign == true) {
			op = unequal_location;
			op_type = 'n';
		}
		if (op == p) {
			// deal with star or minus sign
			int sum = 0;
			for (int i = p; i <= q; i++) {
				if (tokens[i].type == STAR) {
					sum++;
				}
				else {
					break;
				}
			}
			if (sum != 0) {
				uint32_t val;
				val = eval(p + sum, q);
				for (i = 0; i < sum; i++) {
					val = vaddr_ifetch(val, 4);
				}
				return val;
			}
			else {
				for (int i = p; i <= q; i++) {
					if (tokens[i].type == MINUS) {
						sum++;
					}
					else {
						break;
					}
				}
				if (sum != 0) {
					uint32_t val;
					val = eval(p + sum, q);
					if (sum & 1) return val * -1;
					else return val;
				}
			}
		}
		uint32_t val1 = eval(p, op - 1);
		int bj = 0;
		if (op_type == '&' && val1 == 0 && type1_sign == false && type2_sign == false) {
			bj = 1;
		}
		uint32_t val2 = eval(op + 1, q);
		if (bj == 1 && type1_sign == false) {
			type2_sign = false;
		}
	 	switch (op_type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/':
					  if (val2 == 0) {
						  type2_sign = true; 
						  return 0;
					  }
					  else {
						  return val1 / val2;
					  }
			case TK_EQ: return val1 == val2;
			case 'n': return val1 != val2;
			case '&': return val1 && val2;
			default: return 0;
		} 
	} 
}
void expr_clear(){
	int i;
	for (i = 0; i < nr_token; i++) {
		tokens[i].type = ' ';
		memset(tokens[i].str,0 , sizeof(tokens[i].str));
		top = 0;
		st[i] = 0;
		match_parentheses[i] = 0;	
	} 
	nr_token = 0;

}
word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  type1_sign = false;
  type2_sign = false;
  pre_check();
  *success = true;
  int i;
  for (i = 0; i < nr_token; i++) {
	  if ( (tokens[i].type == '*'  // 9.23 ignore nimus 
		  ||tokens[i].type == '-') && (i == 0 || tokens[i - 1].type == '+'
	                                          || tokens[i - 1].type == '-'
		  								      || tokens[i - 1].type == '*'
										      || tokens[i - 1].type == '/'
										   || tokens[i - 1].type == '('
										   || tokens[i - 1].type == TK_EQ
										   || tokens[i - 1].type == 'n'
										   || tokens[i - 1].type == '&'
										   || tokens[i - 1].type == STAR
								    	   || tokens[i - 1].type == MINUS
	    )) {
		  if (tokens[i].type == '*') {
			tokens[i].type = STAR;
		  }
		  else {	
			tokens[i].type = MINUS;
		  }
	  }
  }
  uint32_t answer = eval(0, nr_token - 1);
  expr_clear();
  if (type1_sign == true) {
		*success = false;
		return 1;
  }
  if (type2_sign == true) {
		*success = false;
		return 2;	  
  }
  return answer;
}
