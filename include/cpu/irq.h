#ifndef _CPU_IRQ_H_
#define _CPU_IRQ_H_

#include <stdint.h>
#include <cpu/interrupt.h>

void irq_install();
void irq_install_handler(uint32_t irq, void(*handler)(struct interrupt_frame* frame));
void irq_uninstall_handler(uint32_t irq);

#endif
