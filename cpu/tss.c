#include <cpu/tss.h>
#include <cpu/gdt.h>
#include <cenux/string.h>

struct tss tss;

void tss_set_gate(uint32_t seg, uint32_t esp0, uint32_t ss0)
{
	memset(&tss, 0x00, sizeof(tss));

	gdt_set_gate(seg, (uint32_t)&tss, sizeof(tss), 0xE9, 0x00);

	// default kernel sp and ss - When we switch from userland to kernel land, these values will be set
	tss.esp0 = esp0;
	tss.ss0 = ss0;

	/* These segments will be loaded when processor switches to kernel mode
	 * Kernel Code and Data segments with last two bits set. (0x08 => 0x0b, 0x10 => 0x13)
	 * Setting these bits sets the (request privilege level) to 3
	 * RPL => 3 means this TSS can be used to switch to kernel mode from ring 3
	 */
	tss.cs = 0x0b;
	tss.ss = tss.ds = tss.es = tss.fs = tss.gs = 0x13;
}

void tss_set_esp0(uint32_t esp0)
{
	tss.esp0 = esp0;
}
