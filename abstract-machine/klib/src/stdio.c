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
	fmt = fmt + concat(deal_, type)(fmt);

int deal_width(const char *fmt){ return 0;}
int deal_character(const char *fmt){ return 0;}
int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int d, len = 0;
  int width = 0;
//  char width_type = ' ';
  char *s;
  while (*fmt != '\0') {
	switch(*fmt) {
      case '%':
		fmt++;
		CASE(fmt, width)
		CASE(fmt, character)
		if (*fmt == '0') {
			fmt++;
			while(*fmt <= '9' && *fmt >= '0') {
				width = width * 10 + *fmt;
				fmt++;
			}
		}
		switch(*fmt) {
			case 'd':
				d = va_arg(ap, int);
				int tmpd =  my_atoi(NULL, d, 1, 1);
				fmt++;
			    len += tmpd;
				break;
			case 'c':
			    d = va_arg(ap, int);
				putch(d);
				fmt++;
			    len += 1;
				break;
 
			case 's':
				s = va_arg(ap, char *);
				for (int i = 0; s[i] != '\0'; i++) putch(s[i]);
				int tmps = strlen(s);
			     fmt++;
				len += tmps;
				break;
			default:
				putch('%');	putch(*fmt);
			    fmt++;
				len += 2;
				break;
		}
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
