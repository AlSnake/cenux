#include <fs/ext2/superblock.h>
#include <kernel/kprintf.h>

int32_t ext2_superblock_read(struct device* dev, struct ext2_superblock* sb)
{
	if(!dev->read) {
		kprintf("EXT2 DEV: NO READ OPERATION\n");
		return -1;
	}

	if(dev->read(dev, 2, sb, sizeof(struct ext2_superblock)) == -1) 
		return -1;

	if(sb->signature != EXT2_SIGNATURE) {
		kprintf("INVALID EXT2 SIGNATURE\n");
		return -1;
	}

	return 0;
}

void ext2_superblock_dump(struct ext2_superblock* sb)
{
	kprintf("EXT2 sb->superblock_id = %d\n", sb->superblock_id);
	kprintf("EXT2 sb->signature: %x\n", sb->signature);
	kprintf("EXT2 sb->block_size: %d\n", 1024 << sb->block_size);
	kprintf("EXT2 sb->inode_size = %d\n", sb->inode_size);
	kprintf("EXT2 sb->blocks_per_group = %d\n", sb->blocks_per_group);
	kprintf("EXT2 sb->inodes_per_group = %d\n", sb->inodes_per_group);
	kprintf("EXT2 sb->block_count = %d\n", sb->block_count);
	kprintf("EXT2 sb->inode_count = %d\n", sb->inode_count);
	kprintf("EXT2 sb->unallocated_blocks = %d\n", sb->unallocated_blocks);
	kprintf("EXT2 sb->unallocated_inodes = %d\n", sb->unallocated_inodes);
	kprintf("EXT2 sb->major = %d\n", sb->major);
}

size_t ext2_superblock_blocksize(struct ext2_superblock* sb)
{
	return 1024 << sb->block_size;
}

size_t ext2_superblock_blockgroups(struct ext2_superblock* sb)
{
	return (sb->block_count - sb->superblock_id + (sb->blocks_per_group - 1)) / sb->blocks_per_group;
}

size_t ext2_superblock_inodesize(struct ext2_superblock* sb)
{
	return (sb->major == 0) ? 128 : sb->inode_size;
}
