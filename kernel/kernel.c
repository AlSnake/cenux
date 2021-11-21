#include <kernel/kernel.h>
#include <kernel/tty.h>
#include <kernel/kprintf.h>
#include <cpu/idt.h>
#include <kernel/pic.h>
#include <mm/kmem.h>
#include <mm/paging.h>
#include <cpu/gdt.h>

static struct page_directory *kernel_page;

void kmain()
{
	terminal_init();
	gdt_init();
	pic_init();
	kmem_init();
	idt_init();

	kernel_page = paging_alloc(PAGING_IS_PRESENT | PAGING_IS_WRITEABLE |
				   PAGING_ACCESS_FROM_ALL);
	paging_switch(kernel_page);

	enable_paging();
	enable_interrupts();

	kprintf("CENUX OPERATING SYSTEM\nBY ENFORCER INC\n");
}
