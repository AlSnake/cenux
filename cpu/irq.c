#include <cpu/irq.h>
#include <cpu/idt.h>
#include <kernel/kernel.h>
#include <cpu/interrupt.h>
#include <kernel/pic.h>
#include <cpu/ports.h>

/* IRQ FUNCTIONS */
extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

void irq_install()
{
	idt_set_gate(32, (uint32_t)_irq0, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(33, (uint32_t)_irq1, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(34, (uint32_t)_irq2, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(35, (uint32_t)_irq3, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(36, (uint32_t)_irq4, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(37, (uint32_t)_irq5, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(38, (uint32_t)_irq6, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(39, (uint32_t)_irq7, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(40, (uint32_t)_irq8, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(41, (uint32_t)_irq9, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(42, (uint32_t)_irq10, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(43, (uint32_t)_irq11, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(44, (uint32_t)_irq12, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(45, (uint32_t)_irq13, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(46, (uint32_t)_irq14, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(47, (uint32_t)_irq15, KERNEL_CODE_SELECTOR, 0x8E);
}

void _irq_handler(struct interrupt_frame frame)
{
	/* INT >= 40 (meaning IRQ8 - 15), then we need to send an EOI to the slave controller */
	if (frame.int_no >= 40)
		outb(PIC2_COMMAND, PIC_EOI);

	/* In either case, send EOI to Master Interrupt Controller */
	outb(PIC1_COMMAND, PIC_EOI);
}
