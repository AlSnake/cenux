#ifndef _CPU_ISR_H_
#define _CPU_ISR_H_

#include <stdint.h>

/* Stack Registers bottom -> top */
struct interrupt_frame
{
	uint32_t ds; // Data Segment Selector
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
	uint32_t int_no, err_code; // Interrupt number and error code
	uint32_t eip, cs, eflags, pesp, ss; // Pushed by processor
};

void isr_install();

#endif
