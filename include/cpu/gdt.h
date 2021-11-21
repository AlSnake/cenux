#ifndef _CPU_GDT_H_
#define _CPU_GDT_H_

#include <stdint.h>

#define GDT_TOTAL_SEGMENTS 6

struct gdt_desc {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_mid;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
}__attribute__((packed));

struct gdtr_desc {
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));

void gdt_init();
void gdt_set_gate(uint32_t seg, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);

#endif
