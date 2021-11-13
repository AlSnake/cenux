#include <kernel/kernel.h>
#include <kernel/tty.h>
#include <kernel/vga.h>
#include <stddef.h>

size_t strlen(const char *str)
{
	size_t len = 0;
	while (str[len++] != '\0')
		;
	return len;
}

void printk(const char *str)
{
	size_t len = strlen(str);
	for (size_t i = 0; i < len; i++)
		terminal_writechar(str[i], VGA_COLOR_WHITE);
}

void kmain()
{
	terminal_initalize();
	printk("CENUX OPERATING SYSTEM\nBY ENFORCER INC\nGPL");
}
