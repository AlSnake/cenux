#include <cenux/string.h>

size_t strlen(const char *s)
{
	size_t len = 0;
	while (s[len])
		len++;
	return len;
}

void *memset(void *s, int c, size_t n)
{
	unsigned char *buf = (unsigned char *)s;
	for (size_t i = 0; i < n; i++)
		buf[i] = (unsigned char)c;
	return s;
}
