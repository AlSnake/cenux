#include <cpu/isr.h>
#include <cpu/idt.h>
#include <kernel/kernel.h>
#include <kernel/panic.h>

/* ISRs reserved for CPU exceptions */
extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();

// 32 Exception Messages
char *exception_messages[] = { "Division By Zero",
			       "Debug",
			       "Non Maskable Interrupt",
			       "Breakpoint",
			       "Into Detected Overflow",
			       "Out of Bounds",
			       "Invalid Opcode",
			       "No Coprocessor",
			       "Double Fault",
			       "Coprocessor Segment Overrun",
			       "Bad TSS",
			       "Segment Not Present",
			       "Stack Fault",
			       "General Protection Fault",
			       "Page Fault",
			       "Unknown Interrupt",
			       "Coprocessor Fault",
			       "Alignment Check",
			       "Machine Check",
			       "Reserved",
			       "Reserved",
			       "Reserved",
			       "Reserved",
			       "Reserved",
			       "Reserved",
			       "Reserved",
			       "Reserved",
			       "Reserved",
			       "Reserved",
			       "Reserved",
			       "Reserved",
			       "Reserved" };

/* Install ISRS */
void isr_install()
{
	idt_set_gate(0, (uint32_t)_isr0, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(1, (uint32_t)_isr1, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(2, (uint32_t)_isr2, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(3, (uint32_t)_isr3, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(4, (uint32_t)_isr4, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(5, (uint32_t)_isr5, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(6, (uint32_t)_isr6, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(7, (uint32_t)_isr7, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(8, (uint32_t)_isr8, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(9, (uint32_t)_isr9, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(10, (uint32_t)_isr10, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(11, (uint32_t)_isr11, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(12, (uint32_t)_isr12, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(13, (uint32_t)_isr13, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(14, (uint32_t)_isr14, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(15, (uint32_t)_isr15, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(16, (uint32_t)_isr16, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(17, (uint32_t)_isr17, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(18, (uint32_t)_isr18, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(19, (uint32_t)_isr19, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(20, (uint32_t)_isr20, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(21, (uint32_t)_isr21, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(22, (uint32_t)_isr22, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(23, (uint32_t)_isr23, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(24, (uint32_t)_isr24, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(25, (uint32_t)_isr25, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(26, (uint32_t)_isr26, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(27, (uint32_t)_isr27, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(28, (uint32_t)_isr28, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(29, (uint32_t)_isr29, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(30, (uint32_t)_isr30, KERNEL_CODE_SELECTOR, 0x8E);
	idt_set_gate(31, (uint32_t)_isr31, KERNEL_CODE_SELECTOR, 0x8E);
}

void _fault_handler(struct interrupt_frame frame)
{
	if (frame.int_no < 32)
		panic(exception_messages[frame.int_no]);
}
