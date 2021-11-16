#include <kernel/pic.h>
#include <cpu/ports.h>
#include <stdint.h>

/* MAP IRQ 0-15 TO IDT ENTRIES 32-47 */
#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

/* IRQs 0 to 7 are mapped to reserved IDT entries 8 to 15 (PROBLEM). REMAP IRQ 0-15 TO IDT ENTRIES 32-47 */
void pic_init()
{
	uint8_t a1, a2;

	// Save Masks
	a1 = insb(PIC1_DATA);
	a2 = insb(PIC2_DATA);

	// Setup Master PIC
	outb(PIC1_COMMAND,
	     ICW1_INIT |
		     ICW1_ICW4); // starts the initialization sequence (in cascade mode)
	outb(PIC1_DATA, PIC1_OFFSET); // ICW2: Master PIC Vector Offset
	outb(PIC1_DATA,
	     4); // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100) (TELL MASTER HE HAS A SLAVE)
	outb(PIC1_DATA, ICW4_8086); // x86 MODE

	// Setup Slave PIC
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	outb(PIC2_DATA, PIC2_OFFSET);
	outb(PIC2_DATA,
	     2); // ICW3: tell Slave PIC its cascade identity (0000 0010)
	outb(PIC2_DATA, ICW4_8086);

	// Restore Saved Masks
	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}

void pic_disable()
{
	outb(PIC1_DATA, 0xFF);
}
