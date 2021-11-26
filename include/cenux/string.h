#ifndef _CENUX_STRING_H_
#define _CENUX_STRING_H_

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* s);
void strrev(char* s, uint32_t len);

void* memset(void* s, int c, size_t n);

#endif
