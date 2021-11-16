#include <cpu/idt.h>
#include <kernel/kernel.h>
#include <kernel/printk.h>
#include <string.h>
#include <cpu/isr.h>
#include <cpu/irq.h>

struct idt_desc idt[CENUX_MAX_INTERRUPTS];
struct idtr_desc idtr;

void idt_init()
{
	memset(idt, 0, sizeof(idt));

	idtr.limit = sizeof(idt) - 1;
	idtr.base = (uint32_t)idt;

	isr_install();
	irq_install();

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
