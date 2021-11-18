#include <kernel/kprintf.h>
#include <kernel/tty.h>

void kprintf(const char *str)
{
	terminal_writestring(str);
}
