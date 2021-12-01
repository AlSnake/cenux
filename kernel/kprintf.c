#include <kernel/kprintf.h>
#include <drivers/vga.h>
#include <cenux/stdlib.h>

static void kprint(const char *str)
{
	vga_print(str);
}

static void kputc(char c)
{
	vga_putchar(c);
}

int32_t kprintf(const char *fmt, ...)
{
	va_list ap;
	const char *ptr;
	char *c;
	int32_t val;

	va_start(ap, fmt);
	for (ptr = fmt; *ptr; ptr++) {
		if (*ptr != '%') {
			kputc(*ptr);
			continue;
		}

		switch (*++ptr) {
		case 'd':
		case 'u':
		case 'i': {
			val = va_arg(ap, int);
			char tmp[12];
			itoa(val, tmp, sizeof(tmp), 10);
			kprint(tmp);
			break;
		}
		case 'x': {
			val = va_arg(ap, int);
			char tmp[12];
			itoa(val, tmp, sizeof(tmp), 16);
			kprint(tmp);
			break;
		}
		case 'c': {
			char ch = va_arg(ap, int);
			kputc(ch);
			break;
		}
		case 's':
			c = va_arg(ap, char *);
			kprint(c);
			break;
		default:
			kputc(*ptr);
			break;
		}
	}
	va_end(ap);

	return 0;
}
