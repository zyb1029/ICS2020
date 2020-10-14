#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
//  assert(0);
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
 // assert(0);
  return 0;
}

int my_atoi(char *dst, int d, int type) {
	if (type == 1) {
		char s[20];
		int len = 0;
		if (d == 0) {
			len = 1;
			*dst = 0 + '0';	
		}
        else {
			while (d) {
			 s[len++] = d % 10 + '0';	
			 d = d / 10;	
			}
			for (int i = len - 1; i >= 0; i--) {
				*(dst + len - 1 -i) = *(s + i);
			}
		}
		return len;
	}	
	else return 0;
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
				int tmpd =  my_atoi(out, d, 1);
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
