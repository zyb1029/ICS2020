#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
// finish testing
size_t strlen(const char *s) {
  size_t ans = 0;
  for (int i = 0; s[i] != 0; i++) ans++;
  return ans;
}

// finish testing
char *strcpy(char* dst,const char* src) {
	assert(dst != NULL && src != NULL);
	int i;
	for (i = 0; *(src + i) != '\0'; i++) {
		*(dst + i) = *(src + i);
	}
	*(dst + i) = '\0';
	return dst;
}
// finish testing
char* strncpy(char* dst, const char* src, size_t n) {
	size_t i;
	for (i = 0; i < n && src[i] != '\0'; i++){
		dst[i] = src[i];
	}
	for( ; i < n; i++)
		dst[i] = '\0';
	return dst;
}
// finish testing
char* strcat(char* dst, const char* src) { 
	assert (dst != NULL && src != NULL);
	int i;
	for (i = 0; *(dst + i) != '\0'; i++);
	int j;
	for (j = 0; *(src + j) != '\0'; j++) {
		*(dst + i + j) = *(src + j);
	}
	*(dst + i + j) = '\0';
	return dst;
}
// finish testing
int strcmp(const char* s1, const char* s2) {
	assert (s1 != NULL && s2 != NULL);
	while (*s1 == *s2) {
		if (*s1 == '\0') {
			break;	
		}
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  return 0;
}
// finsh testing
void* memset(void* v,int c,size_t n) {printf("122\n");
  unsigned char *dst = (unsigned char *)v;
  for (int i = 0; i < n; i++) {
	  *(dst + i) = c;	  
  }
  return v;
}
// finsish testing
void* memmove(void* dst,const void* src,size_t n) {
	unsigned char s[65536];
	unsigned char *s1 = (unsigned char *)dst;
	unsigned char *s2 = (unsigned char *)src;
	for (int i = 0; i < n; i++) s[i] = s2[i];
	for (int i = 0; i < n; i++) s1[i] = s[i];
	return dst;
}

// finish testing
void* memcpy(void* out, const void* in, size_t n) {
	unsigned char *s1 = (unsigned char *)out;
	unsigned char *s2 = (unsigned char *)in;
	for (int i = 0; i < n; i++) *(s1 + i) = *(s2 + i);
    return out;
}

int memcmp(const void* s1, const void* s2, size_t n) {
  if (n == 0) {
	return 0;	  
  }
  unsigned char *dst = (unsigned char *)s1;
  unsigned char *src = (unsigned char *)s2; 
  int i;
  for (i = 0; i < n && *dst == *src; i++) {
	dst++;
	src++;	  
  }
  if (i == n) return 0;
  else return *dst - *src;
}

#endif
