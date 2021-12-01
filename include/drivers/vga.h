#ifndef _DRIVERS_VGA_H_
#define _DRIVERS_VGA_H_

#include <stdint.h>

/* REG PORTS */
#define VGA_CTRL_PORT	0x3D4
#define VGA_DATA_PORT	0x3D5

/* SCREEN */
#define VGA_WIDTH	80
#define VGA_HEIGHT	25

/* COLORS */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15
};

void vga_init();
void vga_putchar(uint8_t c);
void vga_putchar_color(uint8_t c, uint8_t color);
void vga_print(const char *str);
void vga_cls();
uint32_t vga_get_cursor();
void vga_set_cursor(uint32_t offset);
uint8_t vga_get_color();
void vga_set_color(uint8_t color);

#endif
