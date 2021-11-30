#ifndef _DRIVERS_KEYBOARD_H_
#define _DRIVERS_KEYBOARD_H_

/* PORTS */
#define PS2_COMMAND_PORT		0x64
#define PS2_DATA_PORT			0x60

/* COMMANDS */
#define PS2_CMD_ENABLE_PORT1		0xAE
#define PS2_CMD_DISABLE_PORT1		0xAD

void keyboard_init();

#endif
