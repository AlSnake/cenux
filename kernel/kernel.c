#include <kernel/kernel.h>
#include <drivers/vga.h>
#include <kernel/kprintf.h>
#include <cpu/idt.h>
#include <kernel/pic.h>
#include <mm/kmem.h>
#include <mm/paging.h>
#include <cpu/gdt.h>
#include <drivers/ata.h>
#include <drivers/device.h>
#include <kernel/pit.h>
#include <drivers/keyboard.h>
#include <fs/vfs.h>

static struct page_directory *kernel_page = 0;

void kmain()
{
	vga_init();
	gdt_init();
	pic_init();
	kmem_init();
	idt_init();
	// pit_init(100); // 100ms
	device_init();
	ata_init();
	vfs_init();
	// keyboard_init();

	kernel_page = paging_alloc(PAGING_IS_PRESENT | PAGING_IS_WRITEABLE |
				   PAGING_ACCESS_FROM_ALL);
	paging_switch(kernel_page);

	enable_paging();
	enable_interrupts();

	// _device_print();
}
