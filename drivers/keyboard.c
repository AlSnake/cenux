#include <drivers/keyboard.h>
#include <cpu/interrupt.h>
#include <cpu/irq.h>
#include <cpu/ports.h>
#include <stdint.h>
#include <stddef.h>
#include <kernel/kprintf.h>

#define KEY_RELEASED 0x80

uint16_t keystatus = 0;

enum KEYCODE {
	NULL_KEY = 0,
	A_PRESSED = 0x1E,
	A_RELEASED = 0x9E,
	B_PRESSED = 0x30,
	B_RELEASED = 0xB0,
	C_PRESSED = 0x2E,
	C_RELEASED = 0xAE,
	D_PRESSED = 0x20,
	D_RELEASED = 0xA0,
	E_PRESSED = 0x12,
	E_RELEASED = 0x92,
	F_PRESSED = 0x21,
	F_RELEASED = 0xA1,
	G_PRESSED = 0x22,
	G_RELEASED = 0xA2,
	H_PRESSED = 0x23,
	H_RELEASED = 0xA3,
	I_PRESSED = 0x17,
	I_RELEASED = 0x97,
	J_PRESSED = 0x24,
	J_RELEASED = 0xA4,
	K_PRESSED = 0x25,
	K_RELEASED = 0xA5,
	L_PRESSED = 0x26,
	L_RELEASED = 0xA6,
	M_PRESSED = 0x32,
	M_RELEASED = 0xB2,
	N_PRESSED = 0x31,
	N_RELEASED = 0xB1,
	O_PRESSED = 0x18,
	O_RELEASED = 0x98,
	P_PRESSED = 0x19,
	P_RELEASED = 0x99,
	Q_PRESSED = 0x10,
	Q_RELEASED = 0x90,
	R_PRESSED = 0x13,
	R_RELEASED = 0x93,
	S_PRESSED = 0x1F,
	S_RELEASED = 0x9F,
	T_PRESSED = 0x14,
	T_RELEASED = 0x94,
	U_PRESSED = 0x16,
	U_RELEASED = 0x96,
	V_PRESSED = 0x2F,
	V_RELEASED = 0xAF,
	W_PRESSED = 0x11,
	W_RELEASED = 0x91,
	X_PRESSED = 0x2D,
	X_RELEASED = 0xAD,
	Y_PRESSED = 0x2C,
	Y_RELEASED = 0xAC,
	Z_PRESSED = 0x15,
	Z_RELEASED = 0x95,

	ZERO_PRESSED = 0x29,
	ONE_PRESSED = 0x2,
	TWO_PRESSED = 0x3,
	THREE_PRESSED = 0x4,
	FOUR_PRESSED = 0x5,
	FIVE_PRESSED = 0x6,
	SIX_PRESSED = 0x7,
	SEVEN_PRESSED = 0x8,
	EIGHT_PRESSED = 0x9,
	NINE_PRESSED = 0xA,

	DOT_PRESSED = 0x34,
	DOT_RELEASED = 0xB4,
	SLASH_PRESSED = 0x35,
	SLASH_RELEASED = 0xB5,
	BACKSPACE_PRESSED = 0xE,
	BACKSPACE_RELEASED = 0x8E,
	SPACE_PRESSED = 0x39,
	SPACE_RELEASED = 0xB9,
	ENTER_PRESSED = 0x1C,
	ENTER_RELEASED = 0x9C,
	LSHIFT_PRESSED = 0x2A,
	LSHIFT_RELEASED = 0xAA,
	LCTRL_PRESSED = 0x1D,
	LCTRL_RELEASED = 0x9D,
	LALT_PRESSED = 0x38,
	LALT_RELEASED = 0xB8,
};

uint8_t keyscan[256] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	LCTRL_PRESSED, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
	'`', LSHIFT_PRESSED, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.',
	'/', LSHIFT_PRESSED, '*',
	LALT_PRESSED, // Alt        -56
	' ', // Space
	0, // Caps Lock
	0, // F1
	0, // F2
	0, // F3
	0, // F4
	0, // F5
	0, // F6
	0, // F7
	0, // F8
	0, // F9
	0, // F10
	0, // Num Lock
	0, // Scroll Lock
	0, // Home
	0, // Up Arrow
	0, // Page Up
	'-',
	0, // Left Arrow
	0,
	0, // Right Arrow
	'+',
	0, // End
	0, // Down Arrow
	0, // Page Down
	0, // Insert
	0, // Delete
	0, 0, 0,
	0, // F11
	0, // F12
	0, // Undefined    -89
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// Upper keyboard (>128)
	0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
	LCTRL_PRESSED, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"',
	'~', LSHIFT_PRESSED, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>',
	'?', LSHIFT_PRESSED, '*', LALT_PRESSED, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static uint8_t keyboard_to_ascii(uint8_t scancode)
{
	size_t size = sizeof(keyscan);
	return (scancode > size) ? 0 : keyscan[scancode];
}

static void keyboard_handler(struct interrupt_frame *frame)
{
	uint8_t scancode = insb(PS2_DATA_PORT);
	uint8_t key = keyboard_to_ascii(scancode);

	if (scancode & KEY_RELEASED) {
		if (key == LSHIFT_PRESSED)
			keystatus &= ~0x0100;
		else if (key == LCTRL_PRESSED)
			keystatus &= ~0x0010;
		else if (key == LALT_PRESSED)
			keystatus &= ~0x0001;
		return;
	}

	if (key == LSHIFT_PRESSED)
		keystatus |= 0x0100;
	else if (key == LCTRL_PRESSED)
		keystatus |= 0x0010;
	else if (key == LALT_PRESSED)
		keystatus |= 0x0001;

	if ((keystatus & 0x0100) == 0x0100)
		kprintf("%c", keyscan[scancode + 128]);
	else
		kprintf("%c", key);
}

void keyboard_init()
{
	outb(PS2_COMMAND_PORT, PS2_CMD_ENABLE_PORT1);
	irq_install_handler(1, keyboard_handler);
}
