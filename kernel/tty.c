#include <kernel/tty.h>
#include <kernel/vga.h>

uint16_t *video_mem;
uint16_t terminal_row;
uint16_t terminal_col;

void terminal_putchar(int x, int y, unsigned char c, uint8_t color)
{
	// position = (y_position * characters_per_line) + x_position;
	video_mem[(y * VGA_WIDTH) + x] = vga_entry(c, color);
}

void terminal_writechar(unsigned char c, uint8_t color)
{
	if (c == '\n') {
		terminal_col = 0;
		terminal_row += 1;
		return;
	}

	terminal_putchar(terminal_col, terminal_row, c, color);
	terminal_col += 1;
	if (terminal_col >= VGA_WIDTH) {
		terminal_col = 0;
		terminal_row += 1;
	}
}

void terminal_clear()
{
	for (int y = 0; y < VGA_HEIGHT; y++)
		for (int x = 0; x < VGA_WIDTH; x++)
			terminal_putchar(x, y, ' ', VGA_COLOR_BLACK);
}

void terminal_initalize()
{
	video_mem = (uint16_t *)(0xB8000);
	terminal_row = 0;
	terminal_col = 0;
	terminal_clear();
}
