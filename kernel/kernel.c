#include <kernel/kernel.h>
#include <kernel/tty.h>

void printk(const char *str)
{
	terminal_writestring(str);
}

void kmain()
{
	terminal_initalize();
	printk("CENUX OPERATING SYSTEM\nBY ENFORCER INC\n");
}
