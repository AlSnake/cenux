#include <kernel/kernel.h>
#include <kernel/tty.h>
#include <kernel/printk.h>
#include <cpu/idt.h>
#include <kernel/pic.h>

void kmain()
{
	terminal_init();
	pic_init();
	idt_init();
	enable_interrupts();

	printk("CENUX OPERATING SYSTEM\nBY ENFORCER INC\n");
}
