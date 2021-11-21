#include <cpu/gdt.h>
#include <cenux/string.h>

struct gdt_desc gdt[GDT_TOTAL_SEGMENTS];
struct gdtr_desc gdtr;

extern void gdt_load(struct gdtr_desc *gdtr);

void gdt_init()
{
	memset(gdt, 0x00, sizeof(gdt));

	gdtr.limit = sizeof(gdt) - 1;
	gdtr.base = (uint32_t)gdt;

	gdt_set_gate(0, 0, 0, 0, 0); // NULL DESCRIPTOR

	/* Code and Data Descriptor
	* Base Address 0
	* Limit is 4gb
	* 4kb gran
	* 32bit opcodes
	* Selector 0x08 will be our code segment offset
	* Selector 0x10 will be our data segment offset
	*/
	gdt_set_gate(1, 0, 0xffffffff, 0x9a, 0xcf); // Kernel Code Segment
	gdt_set_gate(2, 0, 0xffffffff, 0x92, 0xcf); // Kernel Data Segment

	gdt_load(&gdtr);
}

void gdt_set_gate(int seg, uint32_t base, uint32_t limit, uint8_t access,
		  uint8_t granularity)
{
	/* Setup Base */
	gdt[seg].base_low = (base & 0xffff); //16bit
	gdt[seg].base_mid = (base >> 16) & 0xff; //24bits of base
	gdt[seg].base_high =
		(base >> 24) & 0xff; //32bits of base (16 + 8 + 8 = 32bits);

	/* Setup Segment Limits */
	gdt[seg].limit_low = (limit & 0x0000ffff); // 16bits of limit
	gdt[seg].granularity =
		((limit >> 16) &
		 0x0F); // 4bits of limit (16 + 4 = 20bits MAX SIZE)

	/* Setup granularity */
	gdt[seg].granularity |=
		(granularity &
		 0xF0); // Toggle first two bits of flags. (b1 = Granularity, b2=32bit pm segment, b3,4=AVL)
	gdt[seg].access = access; // Present, Privilege, Type Flags
}
