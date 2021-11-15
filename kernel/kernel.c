#include <kernel/kernel.h>
#include <kernel/tty.h>
#include <kernel/printk.h>
#include <cpu/idt.h>

void kmain()
{
	terminal_init();
	idt_init();

	printk("CENUX OPERATING SYSTEM\nBY ENFORCER INC\n");
}
