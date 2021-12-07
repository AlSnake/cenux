#ifndef _CENUX_STRING_H_
#define _CENUX_STRING_H_

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* s);
size_t strnlen(const char* s, uint32_t maxlen);
void strrev(char* s, uint32_t len);
char* strchr(const char *s, char c);
size_t strspn(const char *s1, const char *s2);
size_t strcspn(const char *s1, const char *s2);
char* strtok(char *s, const char *delim);
char* strtok_r(char *s, const char *delim, char **saveptr);
char* strncpy(char *s1, const char *s2, size_t n);
int32_t strncmp(const char *s1, const char *s2, size_t n);
char* strdup(const char *s);
char* strndup(const char *s, size_t n);

void* memset(void* s, int32_t c, size_t n);
void* memsetw(uint16_t* s, uint16_t c, size_t n);
int32_t memcmp(const void *s1, const void *s2, size_t n);
void* memcpy(void *dest, const void * src, size_t n);

#endif
