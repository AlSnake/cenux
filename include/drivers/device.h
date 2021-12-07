#ifndef _DRIVERS_DEVICE_H_
#define _DRIVERS_DEVICE_H_

#include <stdint.h>
#include <drivers/devs.h>

typedef enum device_type{
	CHAR_DEVICE = 1,
	BLOCK_DEVICE = 2
}device_type_t;

struct device {
	char* name;
	uint32_t uid;
	device_type_t type;
	int32_t (*read)(struct device* dev, uint32_t offset, void* buf, uint32_t len);
	int32_t (*write)(struct device* dev, uint32_t offset, void* buf, uint32_t len);
	void* priv;
};

void device_init();
uint32_t device_add(struct device* dev);
struct device* device_get(uint32_t uid);
uint32_t device_generate_uid();
void _device_print();

#endif
