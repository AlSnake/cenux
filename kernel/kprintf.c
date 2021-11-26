#include <kernel/kprintf.h>
#include <kernel/tty.h>
#include <kernel/vga.h>
#include <cenux/stdlib.h>

int32_t kprintf(const char *fmt, ...)
{
	va_list ap;
	const char *ptr;
	char *c;
	int32_t val;

	va_start(ap, fmt);
	for (ptr = fmt; *ptr; ptr++) {
		if (*ptr != '%') {
			terminal_writechar(*ptr, VGA_COLOR_WHITE);
			continue;
		}

		switch (*++ptr) {
		case 'd':
		case 'u':
		case 'i': {
			val = va_arg(ap, int);
			char tmp[12];
			itoa(val, tmp, sizeof(tmp), 10);
			terminal_writestring(tmp);
			break;
		}
		case 'x': {
			val = va_arg(ap, int);
			char tmp[12];
			itoa(val, tmp, sizeof(tmp), 16);
			terminal_writestring(tmp);
			break;
		}
		case 's':
			c = va_arg(ap, char *);
			terminal_writestring(c);
			break;
		default:
			terminal_writechar(*ptr, VGA_COLOR_WHITE);
			break;
		}
	}
	va_end(ap);

	return 0;
}
