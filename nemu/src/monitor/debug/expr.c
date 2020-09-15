#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,

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
  {"-", '-'},
  {"\\*", '*'},
  {"/",'/'},
  {"[0-9]+", '0'},
  {"\\(", '('},
  {"\\)", ')'},
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

static Token tokens[32] __attribute__((used)) = {};
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

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

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
					 memcpy(tokens[nr_token].str, substr_start,substr_len);
					 tokens[nr_token].type = rules[i].token_type;
					 break;
			default: TODO();
        }
		nr_token = nr_token + 1;
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
int match_parentheses[32];

int st[32], top = 0;

void pre_check(){
	for (int i = 0; i < nr_token; i++) {
		if(tokens[i].type == '(') {
			st[++top] = i;
		}
		else if(tokens[i].type == ')') {
			if(top == 0) {
				assert(0);
			}
			else {
				match_parentheses[st[top--]] = i;
			}
		}
	}
	if(top) {
		assert(0);
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
word_t eval(int p, int q) {
	printf("%d %d\n", p, q);
	if (p > q) {
		printf("%d %d\n",p,q);
		return 0;
		//assert(0);
	}
	else if (p == q) {
		if(tokens[p].type != '0') {
			assert(0);
		}
		else {
			int32_t tmp;
			sscanf(tokens[p].str, "%d", &tmp);
			return tmp;
		}
	}
	else if (p + 1 == q && tokens[p].type == '-' && tokens[q].type == '0') {
		int32_t tmp;
		sscanf(tokens[q].str, "%d", &tmp);
		return -1 * tmp;
	}
	else if (check_parentheses(p, q) == true) {
		return eval(p + 1, q - 1);
	}
    else {
		int i, sum = 0, op = p;
		char op_type = '0';
		for (i = q; i > p; i--) {
			if(tokens[i].type == '(') {
				sum--;
			}
			else if(tokens[i].type == ')') {
				sum++;
			}
			else if(tokens[i].type == '-' && i != q &&tokens[i + 1].type == '0' && i != p && (tokens[i - 1].type == '+' || tokens[i - 1].type == '-' ||tokens[i - 1].type == '*' || tokens[i - 1].type == '/') ) {
				continue;
			}
			else if(sum == 0){
				if(tokens[i].type == '+' || tokens[i].type == '-' || tokens[i].type == '*' || tokens[i].type == '/'){
					if (op_type == '0') {
						op_type = tokens[i].type;
						op = i;
					}
					else if( (op_type == '*' || op_type == '/') && (tokens[i].type == '+' || tokens[i].type == '-')) {
						op_type = tokens[i].type;
						op = i;
					}
				}
			}
		}
		printf("%c %d 666\n",op_type,op);
		int32_t val1 = eval(p, op -1);
		int32_t val2 = eval(op + 1, q);
		switch (op_type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			default: return 0;
		}
	}
}
void expr_clear(){
	int i;
	for (i = 0; i < nr_token; i++) {
		tokens[i].type = ' ';
		memset(tokens[i].str,0 , sizeof(tokens[i].str));
		printf("%s", tokens[i].str);
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
  pre_check();
  *success = true;
  printf("%d\n", nr_token);
  word_t answer = eval(0, nr_token - 1);
  expr_clear();
  return answer;
}
