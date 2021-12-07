#ifndef _FS_VFS_H_
#define _FS_VFS_H_

#include <drivers/device.h>

/* EXT2 FILE SYSTEM STRUCTURE */
struct filesystem {
	const char* name;
	struct device* dev;
	int32_t (*read)(struct device* dev, uint32_t offset, uint8_t* buf, uint32_t len);
	void* priv;
};

/* MOUNT POINT */
struct mountpoint {
	char* point;
	struct device* dev;
};

int32_t vfs_init();
int32_t mount_device(struct device* dev, char* location);

#endif
