#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t ans = 0;
  for (int i = 0; s[i] != 0; i++) ans++;
  return ans;
}

char *strcpy(char* dst,const char* src) {
	assert(dst != NULL && src != NULL);
	int i;
	for (i = 0; *(src + i) != '\0'; i++) {
		*(dst + i) = *(src + i);
	}
	*(dst + i) = '\0';
	return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  assert(0);
  return NULL;
}

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
  assert(0);
  return 0;
}

void* memset(void* v,int c,size_t n) {
  char *dst = (char *)v;
  for (int i = 0; i < n; i++) {
	  *(dst + i) = c;	  
  }
  *(dst + n) = '\0';
  return v;
}

void* memmove(void* dst,const void* src,size_t n) {
  assert(0);
  return NULL;
}

void* memcpy(void* out, const void* in, size_t n) {
  assert(0);
  return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n) {
  assert(0);
  return 0;
}

#endif
