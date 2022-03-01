#include <fs/vfs.h>
#include <kernel/kprintf.h>
#include <drivers/device.h>
#include <fs/ext2/ext2.h>

int32_t vfs_init()
{
	kprintf("INITALIZING VFS\n");
	int32_t status = ext2_init(device_get(ATA_PRIMARY_SLAVE_DRIVE_UID));
	kprintf("EXT2 INIT STATUS: %d\n", status);

	return 0;
}
