#include <kernel/printk.h>
#include <kernel/tty.h>

void printk(const char *str)
{
	terminal_writestring(str);
}
