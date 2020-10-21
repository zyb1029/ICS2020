#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int my_atoi(char *dst, int d, int type, int out_type) {
	if (type == 1) {
		char s[20];
		int len = 0;
		if (d == 0) {
			len = 1;
			if (out_type == 0) *dst = 0 + '0';	
			else if (out_type == 1) putch('0');
		}
        else {
			while (d) {
			 s[len++] = d % 10 + '0';	
			 d = d / 10;	
			}
			if (out_type == 0) {
				for (int i = len - 1; i >= 0; i--) {
					*(dst + len - 1 -i) = *(s + i);
				}
			}
			else if (out_type == 1) {
				for (int i = len - 1; i >= 0; i--) {
					putch(s[i]);	
				}	
			}
		}
		return len;
	}	
	else return 0;
}

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
static char* out;
va_list ap;

int deal_width(const char *fmt){
	int len = 0;
	width = 0;
	if (*fmt == '0') {
		fmt++; len++;
		while(*fmt <= '9' && *fmt >= '0') {
			width = width * 10 + *fmt;
			fmt++; len++;
		}
	}
	return len;
}

static char buff[32];

void deal_number(int x,int len, int mod) {
	if (x) {
		while (x) {
			buff[len++] = x % mod + '0';
			buff[len] = '\0';
			x /= mod;
		}
    }
	else {buff[len++] = '0'; buff[len] = '\0';}

	for (int i = len; i < width; i++) { 
		buff[len++] = '0'; buff[len] = '\0';
	}
	width = 0;
	for (int i = 0; i < len / 2; i++) {
		char tmp = buff[len - 1 - i];
		buff[len - 1 - i] = buff[i];	
		buff[i] = tmp;
	}
	out = buff;
}

void deal_character3() { // c
	int d = va_arg(ap, int);
	buff[0] = d; buff[1] = '\0';
	out = buff;
}

void deal_character4() { // d
	int d = va_arg(ap, int);
    deal_number(d, 0, 10);	
}

void deal_character16() { // s
	out = va_arg(ap, char *);
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
		CHARACTER_CASE(16) // s
		CHARACTER_CASE(24) // x
		default:
			buff[0] = *fmt; buff[1] = '\0';
			out = buff;
    }
	return len;
}

void Print() {
	for (int i = 0; out[i] != '\0'; i++) putch(out[i]);
}

int printf(const char *fmt, ...) {
  int len = 0, len_tmp = 0;
  va_start(ap, fmt);
  while (*fmt != '\0') {
	switch(*fmt) {
      case '%':
		fmt++;
//		CASE(fmt, width)
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
  va_list ap;
  va_start(ap, fmt);
  int d, len = 0;
  char *s;
  while (*fmt != '\0') {
	switch(*fmt) {
      case '%':
		fmt++;
		switch(*fmt) {
			case 'd':
				d = va_arg(ap, int);
				int tmpd =  my_atoi(out, d, 1, 0);
				out = out + tmpd; fmt++;
			    len += tmpd;
				break;
			case 's':
				s = va_arg(ap, char *);
				strcpy(out, s);
				int tmps = strlen(out);
				out = out + tmps; fmt++;
				len += tmps;
				break;
			default:
				*out = '%'; out++;
				*out = *fmt; fmt++;
				len += 2;
		}
		break;
	  default:	
		*out = *fmt;
		out++;
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
