#include <kernel/panic.h>
#include <kernel/kprintf.h>

void panic(const char *s)
{
	kprintf(s);
	while (1) {
	}
}
