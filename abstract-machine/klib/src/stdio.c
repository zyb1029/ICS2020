#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define concat(x, y) x ## y

#define CASE(fmt, type) \
    len_tmp = concat(deal_, type)(fmt); \
	fmt = fmt + len_tmp;\
	len = len + len_tmp;

#define CHARACTER_CASE(type) \
	case type:\
		concat(deal_character, type)();\
		break;

static int width = 0;
static char* _out;
va_list ap;

int deal_width(const char *fmt){
	int len = 0;
	width = 0;
	if (*fmt == '0') {
		fmt++; len++;
		while(*fmt <= '9' && *fmt >= '0') {
			width = width * 10 + *fmt - '0';
			fmt++; len++;
		}
	}
	return len;
}

static char buff[32];
static bool number_sign = false;
void deal_number(int x,int len, int mod) {
	if (x) {
		if (x < 0) {
			number_sign = true;
			x *= -1;
		}
		while (x) {
			buff[len++] = x % mod + '0';
			buff[len] = '\0';
			x /= mod;
		}
    }
	else {buff[len++] = '0'; buff[len] = '\0';}
	int bj = 0;
	for (int i = len; i < width; i++) { 
		bj = 1;
		buff[len++] = '0'; buff[len] = '\0';
	}
	if (number_sign){
		if (bj) buff[len - 1] = '-';
		else { buff[len++] = '-';buff[len] = '\0';}
		number_sign = false;
	}
	for (int i = 0; i < len / 2; i++) {
		char tmp = buff[len - 1 - i];
		buff[len - 1 - i] = buff[i];	
		buff[i] = tmp;
	}
	width = 0;
	_out = buff;
}

void deal_character3() { // c
	int d = va_arg(ap, int);
	buff[0] = d; buff[1] = '\0';
	_out = buff;
}

void deal_character4() { // d
	int d = va_arg(ap, int);
    deal_number(d, 0, 10);	
}

void deal_character19() { // s
	_out = va_arg(ap, char *);
}

void deal_character24() { // x
	int d = va_arg(ap, int);
	deal_number(d, 0, 16);
}

int deal_character(const char *fmt) { 
	int len = 1, bias = *fmt - 'a' + 1;
	switch(bias) {
		CHARACTER_CASE(3) // c
		CHARACTER_CASE(4) // d
		CHARACTER_CASE(19) // s
		CHARACTER_CASE(24) // x
		default:
			buff[0] = *fmt; buff[1] = '\0';
			_out = buff;
    }
	return len;
}

void Print() {
	for (int i = 0; _out[i] != '\0'; i++) putch(_out[i]);
}

int printf(const char *fmt, ...) {
  int len = 0, len_tmp = 0;
  va_start(ap, fmt);
  while (*fmt != '\0') {
	switch(*fmt) {
      case '%':
		fmt++;
    	CASE(fmt, width)
		CASE(fmt, character)
		Print();
		break;
	  default:	
		putch(*fmt);
		fmt++;
		len++;
	}	  
  }
  return len;

}

int vsprintf(char *out, const char *fmt, va_list ap) {
 // assert(0);
  return 0;
}


int sprintf(char *out, const char *fmt, ...) {
  va_start(ap, fmt);
  int len = 0, len_tmp = 0;
  while (*fmt != '\0') {
	switch(*fmt) {
      case '%':
		fmt++;
		CASE(fmt, width)
		CASE(fmt, character)
		strcpy(out, _out);
		out += strlen(_out);
		break;
	  default:	
		*out = *fmt; out++;
		fmt++;
		len++;
	}	  
  }
  *out = *fmt;
  return len;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
 // assert(0);
  return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
 // assert(0);
  return 0;
}

#endif
