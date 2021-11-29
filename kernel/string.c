#include <cenux/string.h>
#include <mm/kmem.h>

size_t strlen(const char *s)
{
	size_t len = 0;
	while (s[len])
		len++;
	return len;
}

size_t strnlen(const char *s, uint32_t maxlen)
{
	size_t len = 0;
	while (s[len] && len < maxlen)
		len++;
	return len;
}

void strrev(char *s, uint32_t len)
{
	for (uint32_t i = 0; i < len / 2; i++) {
		char tmp = s[i];
		s[i] = s[len - 1 - i];
		s[len - 1 - i] = tmp;
	}
}

char *strchr(const char *s, char c)
{
	do {
		if (*s == c)
			return (char *)s;
	} while (*s++);

	return 0;
}

size_t strspn(const char *s1, const char *s2)
{
	size_t len = 0;

	if (!s1 || !s2)
		return 0;

	while (*s1 && strchr(s2, *s1++))
		len++;

	return len;
}

size_t strcspn(const char *s1, const char *s2)
{
	size_t len = 0;

	if (!s1 || !s2)
		return 0;

	while (*s1) {
		if (strchr(s2, *s1++))
			return len;

		len++;
	}

	return len;
}

char *strtok(char *s, const char *delim)
{
	static char *saveptr;
	return strtok_r(s, delim, &saveptr);
}

char *strtok_r(char *s, const char *delim, char **saveptr)
{
	char *end;
	if (!s)
		s = *saveptr;
	if (*s == '\0') {
		*saveptr = s;
		return 0;
	}

	s += strspn(s, delim);
	if (*s == '\0') {
		*saveptr = s;
		return 0;
	}

	end = s + strcspn(s, delim);
	if (*end == '\0') {
		*saveptr = end;
		return s;
	}

	*end = '\0';
	*saveptr = end + 1;
	return s;
}

char *strncpy(char *s1, const char *s2, size_t n)
{
	size_t i = 0;
	for (i = 0; i < n && s2[i] != '\0'; i++)
		s1[i] = s2[i];
	for (; i < n; i++)
		s1[i] = '\0';
	return s1;
}

char *strdup(const char *s)
{
	size_t len = strlen(s) + 1;
	char *dup = kmalloc(len);
	if (dup)
		strncpy(dup, s, len);

	return dup;
}

char *strndup(const char *s, size_t n)
{
	char *dup = kmalloc(n + 1);
	if (dup)
		strncpy(dup, s, n);

	dup[n] = '\0';
	return dup;
}

void *memset(void *s, int32_t c, size_t n)
{
	unsigned char *buf = (unsigned char *)s;
	for (size_t i = 0; i < n; i++)
		buf[i] = (unsigned char)c;
	return s;
}

int32_t memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *p1 = s1;
	const unsigned char *p2 = s2;

	for (size_t i = 0; i < n; i++, p1++, p2++) {
		if (*p1 < *p2)
			return -1;
		else if (*p1 > *p2)
			return 1;
	}
	return 0;
}
