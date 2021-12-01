#include <drivers/vga.h>
#include <cpu/ports.h>
#include <cenux/string.h>

#define VGA_ENTRY(c, color) ((color << 8) | c)
#define VGA_GET_OFFSET(col, row) ((row * VGA_WIDTH + col) * 2)

uint16_t *vga_addr;
uint16_t vga_col;
uint16_t vga_row;
uint8_t vga_color;

static void vga_scroll()
{
	void *start = (void *)vga_addr + 1 * VGA_WIDTH * 2;
	uint32_t size = vga_row * VGA_WIDTH * 2;
	memcpy(vga_addr, start, size);
	start = (void *)vga_addr + size;
	memsetw(start, VGA_ENTRY(' ', vga_color), VGA_WIDTH);
	vga_row--;
}

static void vga_putx(uint32_t col, uint32_t row, uint8_t c, uint8_t color)
{
	// position = (y_position * characters_per_line) + x_position;
	vga_addr[(row * VGA_WIDTH) + col] = VGA_ENTRY(c, color);
}

void vga_init()
{
	vga_addr = (uint16_t *)(0xB8000);
	vga_col = 0;
	vga_row = 0;
	vga_color = VGA_COLOR_WHITE;
	vga_cls();
}

void vga_putchar(uint8_t c)
{
	vga_putchar_color(c, vga_color);
}

void vga_putchar_color(uint8_t c, uint8_t color)
{
	if (vga_row >= VGA_HEIGHT)
		vga_scroll();

	if (c == '\n') {
		vga_col = 0;
		vga_row += 1;
		return;
	}

	vga_putx(vga_col, vga_row, c, color);
	vga_col += 1;
	if (vga_col >= VGA_WIDTH) {
		vga_col = 0;
		vga_row += 1;
	}

	vga_set_cursor(VGA_GET_OFFSET(vga_col, vga_row));
}

void vga_print(const char *str)
{
	size_t len = strlen(str);
	for (size_t i = 0; i < len; i++)
		vga_putchar(str[i]);
}

void vga_cls()
{
	for (uint32_t y = 0; y < VGA_HEIGHT; y++)
		for (uint32_t x = 0; x < VGA_WIDTH; x++)
			vga_putx(x, y, ' ', vga_color);
	vga_set_cursor(0);
}

uint32_t vga_get_cursor()
{
	uint32_t offset = 0;
	outb(VGA_CTRL_PORT, 14);
	offset = insb(VGA_DATA_PORT) << 8;
	outb(VGA_CTRL_PORT, 15);
	offset += insb(VGA_DATA_PORT);
	return offset * 2;
}

void vga_set_cursor(uint32_t offset)
{
	offset /= 2;
	outb(VGA_CTRL_PORT, 14);
	outb(VGA_DATA_PORT, (offset >> 8));
	outb(VGA_CTRL_PORT, 15);
	outb(VGA_DATA_PORT, (offset & 0xff));
}

uint8_t vga_get_color()
{
	return vga_color;
}

void vga_set_color(uint8_t color)
{
	vga_color = color;
}
