#ifndef _CPU_INTERRUPT_H_
#define _CPU_INTERRUPT_H_

#include <stdint.h>

struct interrupt_frame
{
	uint32_t ds; // Data Segment Selector
	uint32_t edi, esi, ebp, iesp, ebx, edx, ecx, eax; // Pushed by pusha
	uint32_t int_no, err_code; // Interrupt number and error code
	uint32_t eip, cs, eflags, esp, ss; // Pushed by processor
};

#endif
