#ifndef _KERNEL_TTY_H_
#define _KERNEL_TTY_H_

#include <stdint.h>

void terminal_init();
void terminal_clear();
void terminal_putchar(int x, int y, unsigned char c, uint8_t color);
void terminal_writechar(unsigned char c, uint8_t color);
void terminal_writestring(const char* str);

#endif
