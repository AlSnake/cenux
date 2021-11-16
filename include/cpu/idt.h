#ifndef _CPU_IDT_H_
#define _CPU_IDT_H_

#include <stdint.h>

#define CENUX_MAX_INTERRUPTS 512

struct idt_desc {
	uint16_t offset_1; // offset bits 0..15
	uint16_t selector; // a code segment selector in GDT
	uint8_t zero; // unused, set to 0
	uint8_t type_attr; // type and attributes
	uint16_t offset_2; // offset bits 16..31
} __attribute__((packed));

struct idtr_desc {
	uint16_t limit; // size of idt - 1
	uint32_t base; // base addr of start of idt
} __attribute__((packed));

extern void enable_interrupts();
extern void disable_interrupts();
extern void idt_load(struct idtr_desc* idtr);
void idt_init();
void idt_set_gate(int interrupt, uint32_t base, uint16_t selector, uint8_t type_attr);

#endif
