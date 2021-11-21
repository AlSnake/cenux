#ifndef _KERNEL_H_
#define _KERNEL_H_

#define KERNEL_CODE_SELECTOR	0x08
#define KERNEL_DATA_SELECTOR	0x10
#define USER_CODE_SELECTOR 	0x18
#define USER_DATA_SELECTOR	0x20
#define TSS_SELECTOR		0x28

void kmain();

#endif
