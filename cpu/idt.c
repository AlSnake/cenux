#include <cpu/idt.h>
#include <kernel/kernel.h>
#include <kernel/printk.h>
#include <string.h>

struct idt_desc idt[CENUX_MAX_INTERRUPTS];
struct idtr_desc idtr;

void idt_zero()
{
	printk("DIVIDE BY ZERO ERROR\n");
}

void idt_init()
{
	memset(idt, 0, sizeof(idt));

	idtr.limit = sizeof(idt) - 1;
	idtr.base = (uint32_t)idt;

	// 0xEE = 0b11101110 => 4b TYPE AND 4b S DPL P
	idt_set_gate(0, (uint32_t)idt_zero, KERNEL_CODE_SELECTOR, 0xEE);

	idt_load(&idtr);
}

void idt_set_gate(int interrupt, uint32_t base, uint16_t selector,
		  uint8_t type_attr)
{
	idt[interrupt].offset_1 = (base & 0xFFFF);
	idt[interrupt].selector = selector;
	idt[interrupt].zero = 0x00;
	idt[interrupt].type_attr = type_attr;
	idt[interrupt].offset_2 = (base >> 16) & 0xFFFF;
}
