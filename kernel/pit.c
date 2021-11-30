#include <kernel/pit.h>
#include <cpu/ports.h>
#include <cpu/irq.h>
#include <kernel/kprintf.h>

uint32_t pit_ticks = 0;

static void pit_handler(struct interrupt_frame *frame)
{
	pit_ticks++;
	kprintf("PIT TICK: %d\n", pit_ticks);
}

static void pit_interval(uint32_t hz)
{
	uint32_t divisor = 1193180 / hz; // DIVISOR
	outb(0x43, 0x36); // COMMAND PORT
	outb(0x40, divisor & 0xFF); // LOW BYTE
	outb(0x40, divisor >> 8); // HIGH BYTE
}

void pit_init(uint32_t hz)
{
	pit_interval(hz);
	irq_install_handler(0, pit_handler);
}

void pit_wait(uint32_t ticks)
{
	uint32_t _ticks = pit_ticks + ticks;
	while (pit_ticks < _ticks)
		;
}
