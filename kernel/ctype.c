#include <cenux/ctype.h>

uint8_t isdigit(char c)
{
	return c >= 48 && c <= 57;
}

uint8_t isalpha(char c)
{
	return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}
