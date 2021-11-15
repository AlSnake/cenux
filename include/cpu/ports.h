#ifndef _CPU_PORTS_H_
#define _CPU_PORTS_H_

#include <stdint.h>

uint8_t insb(uint16_t port);
uint16_t insw(uint16_t port);
void outb(uint16_t port, uint8_t data);
void outw(uint16_t port, uint16_t data);

#endif
