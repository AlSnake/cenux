#include <fs/vfs.h>
#include <mm/kmem.h>
#include <kernel/kprintf.h>
#include <cenux/string.h>
#include <fs/ext2/ext2.h>

struct mountpoint** mounts = 0;
uint32_t mntid = 0;

#define MAX_MOUNTS 64

static struct device* get_mount_device(const char* point)
{
	for(uint32_t i = 0; i < mntid; i++)
		if(!strncmp(point, mounts[i]->point, strlen(point)))
			return mounts[i]->dev;
	return 0;
}

int32_t vfs_init()
{
	kprintf("INITALIZING VFS\n");

	mounts = kmalloc(sizeof(struct mountpoint*) * MAX_MOUNTS);
	if(!mounts) {
		kprintf("VFS: NOMEM\n");
		return -1;
	}

	struct filesystem* ext2fs = ext2_init(device_get(ATA_PRIMARY_SLAVE_DRIVE_UID));
	kprintf("REGISTERED: %s\n", ext2fs->name);
	mntid = 0;
	return 0;
}

int32_t mount_device(struct device* dev, char* point)
{
	if(!dev)
		return -1;

	if(get_mount_device(point))
		return -1;

	
	return 0;
}
