#include <drivers/device.h>
#include <mm/kmem.h>
#include <kernel/panic.h>
#include <kernel/kprintf.h>

struct device **devs = 0;
uint32_t devid = 0;

#define DEVICE_MAX 128

void device_init()
{
	devs = kzalloc(DEVICE_MAX * sizeof(struct device *));
	if (!devs)
		panic("NOMEM");
	devid = 0;
	kprintf("DEVICE MANAGER INITIALIZED\n");
}

uint32_t device_add(struct device *dev)
{
	devs[devid] = dev;
	kprintf("REGISTERED %s DEVICE: %s UID: %d, DEVID: %d\n",
		dev->type == CHAR_DEVICE ? "CHAR" : "BLOCK", dev->name,
		dev->uid, devid);
	devid++;
	return devid - 1;
}

struct device *device_get(uint32_t uid)
{
	for (uint32_t i = 0; i < devid; i++)
		if (devs[i]->uid == uid)
			return devs[i];
	return 0;
}

uint32_t device_generate_uid()
{
	return devid + 1;
}

void _device_print()
{
	kprintf("DEVICE MANAGER\n");
	for (uint32_t i = 0; i < devid; i++)
		kprintf("  %s DEVICE: %s, UID: %d, DEVID: %d\n",
			devs[i]->type == CHAR_DEVICE ? "CHAR" : "BLOCK",
			devs[i]->name, devs[i]->uid, i);
}
