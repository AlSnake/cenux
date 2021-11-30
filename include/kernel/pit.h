#ifndef _KERNEL_PIT_H_
#define _KERNEL_PIT_H_

#include <stdint.h>

void pit_init(uint32_t hz);
void pit_wait(uint32_t ticks);

#endif
