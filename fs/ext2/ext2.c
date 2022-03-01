#include <fs/ext2/ext2.h>
#include <kernel/kprintf.h>
#include <mm/kmem.h>

#define EXT2_SIGNATURE 0xEF53

int32_t ext2_read_superblock(struct device *dev, struct ext2_superblock *sbuf)
{
	// The superblock starts at LBA 2 relative to the start of the partition. So, if the partition starts at LBA 2048, then the superblock starts at LBA 2050.
	// ASSUME EACH BLOCK SIZE IS 1024, EACH SECTOR = 512, READ 2 SECTORS
	if (dev->read(dev, 2, sbuf, 2) == -1)
		return -1;

	if (sbuf->signature != EXT2_SIGNATURE) {
		kprintf("INVALID EXT2 SIGNATURE\n");
		return -1;
	}

	return 0;
}

int32_t ext2_read_block(struct device *dev, struct ext2_priv *priv,
			uint32_t block, uint8_t *buf)
{
	int sectors_per_block = priv->block_size / 512;
	return dev->read(dev, block * sectors_per_block, buf,
			 sectors_per_block);
}

void ext2_dump_superblock(struct ext2_superblock *sbuf)
{
	kprintf("-----EXT2 DUMP SUPERBLOCK START-----\n");
	kprintf("inode_count = %d\n", sbuf->inode_count);
	kprintf("block_count = %d\n", sbuf->block_count);
	kprintf("reserved_blocks = %d\n", sbuf->reserved_blocks);
	kprintf("unallocated_blocks = %d\n", sbuf->unallocated_blocks);
	kprintf("unallocated_inodes = %d\n", sbuf->unallocated_inodes);
	kprintf("superblock_id = %d\n", sbuf->superblock_id);
	kprintf("block_size = %d\n", 1024 << sbuf->block_size);
	kprintf("fragment_size = %d\n", 1024 << sbuf->fragment_size);
	kprintf("blocks_per_group = %d\n", sbuf->blocks_per_group);
	kprintf("fragments_per_group = %d\n", sbuf->fragments_per_group);
	kprintf("inodes_per_group = %d\n", sbuf->inodes_per_group);
	// kprintf("mount_time = %d\n", sbuf->mount_time);
	// kprintf("write_time = %d\n", sbuf->write_time);
	kprintf("mount_count = %d\n", sbuf->mount_count);
	kprintf("max_mount = %d\n", sbuf->max_mount);
	kprintf("signature = %x\n", sbuf->signature);
	kprintf("state = %d\n", sbuf->state);
	kprintf("error = %d\n", sbuf->error);
	kprintf("minor = %d\n", sbuf->minor);
	kprintf("major = %d\n", sbuf->major);
	// kprintf("last_fsck = %d\n", sbuf->last_fsck);
	kprintf("fsck_interval = %d\n", sbuf->fsck_interval);
	kprintf("os_id = %d\n", sbuf->os_id);
	// kprintf("uid = %d\n", sbuf->uid);
	// kprintf("gid = %d\n", sbuf->gid);
	kprintf("inode = %d\n", sbuf->inode);
	kprintf("inode_size = %d\n", sbuf->inode_size);
	kprintf("blockgroup = %d\n", sbuf->blockgroup);
	// kprintf("last_mounted = %d\n", sbuf->last_mounted);
	kprintf("blockgroup_count = %d\n",
		((sbuf->block_count - sbuf->superblock_id +
		  (sbuf->blocks_per_group - 1)) /
		 sbuf->blocks_per_group));
	kprintf("-----EXT2 DUMP SUPERBLOCK END-----\n");
}

int32_t ext2_read_blockgroup(struct device *dev, struct ext2_priv *priv,
			     struct ext2_blockgroup_desc *bgbuf, uint32_t group)
{
	return ext2_read_block(dev, priv, priv->first_blockgroup + (2 * group),
			       (uint8_t *)bgbuf);
}

void ext2_dump_blockgroup(struct ext2_blockgroup_desc *bgbuf)
{
	kprintf("-----EXT2 DUMP BLOCKGROUP START-----\n");
	kprintf("blocks_bitmap = %d\n", bgbuf->blocks_bitmap);
	kprintf("inodes_bitmap = %d\n", bgbuf->inodes_bitmap);
	kprintf("inode_table = %d\n", bgbuf->inode_table);
	kprintf("unallocated_blocks = %d\n", bgbuf->unallocated_blocks);
	kprintf("unallocated_inodes = %d\n", bgbuf->unallocated_inodes);
	kprintf("dir_count = %d\n", bgbuf->dir_count);
	kprintf("-----EXT2 DUMP BLOCKGROUP END-----\n");
}

void ext2_dump_inode(struct ext2_inode *inodebuf)
{
	kprintf("-----EXT2 DUMP INODE START-----\n");
	kprintf("mode = %d\n", inodebuf->mode);
	kprintf("uid = %d\n", inodebuf->uid);
	kprintf("size_lo = %d\n", inodebuf->size_lo);
	kprintf("atime = %d\n", inodebuf->atime);
	kprintf("ctime = %d\n", inodebuf->ctime);
	kprintf("mtime = %d\n", inodebuf->mtime);
	kprintf("dtime = %d\n", inodebuf->dtime);
	kprintf("gid = %d\n", inodebuf->gid);
	kprintf("hard_links = %d\n", inodebuf->hard_links);
	kprintf("sector_count = %d\n", inodebuf->sector_count);
	kprintf("flags = %d\n", inodebuf->flags);
	kprintf("ossv1 = %d\n", inodebuf->ossv1);
	kprintf("dbp = %d\n", inodebuf->dbp);
	kprintf("sibp = %d\n", inodebuf->sibp);
	kprintf("dibp = %d\n", inodebuf->dibp);
	kprintf("tibp = %d\n", inodebuf->tibp);
	kprintf("fragment_addr = %d\n", inodebuf->fragment_addr);
	kprintf("ossv2 = %d\n", inodebuf->ossv2);
	kprintf("-----EXT2 DUMP INODE END-----\n");
}

int32_t ext2_read_inode(struct device *dev, struct ext2_priv *priv,
			uint32_t inode)
{
	int bg = (inode - 1) / priv->inodes_per_group;
	struct ext2_blockgroup_desc *bgd =
		kzalloc(sizeof(struct ext2_blockgroup_desc));
	if (!bgd)
		return -1;
	ext2_read_blockgroup(dev, priv, bgd, bg);

	ext2_dump_blockgroup(bgd);

	int index = (inode - 1) % priv->inodes_per_group;
	int offset = (index * priv->inode_size) / priv->block_size;

	struct ext2_inode *inodebuf = kzalloc(sizeof(struct ext2_inode));
	ext2_read_block(dev, priv, offset, (uint8_t *)inodebuf);

	ext2_dump_inode(inodebuf);
	return 0;
}

// FIX MEMORY MANAGEMENT
int32_t ext2_init(struct device *dev)
{
	if (!dev || !dev->read) {
		kprintf("EXT2: INVALID DEV");
		return -1;
	}

	struct ext2_superblock *superblock =
		kzalloc(sizeof(struct ext2_superblock));
	if (!superblock)
		return -1;
	if (ext2_read_superblock(dev, superblock) == -1)
		return -1;

	struct ext2_priv *priv = kzalloc(sizeof(struct ext2_priv));
	priv->superblock = superblock;
	priv->dev = dev;
	priv->block_size = (1024 << superblock->block_size);
	priv->inode_size = superblock->inode_size;
	priv->inodes_per_group = superblock->inodes_per_group;
	priv->superblock_id = superblock->superblock_id;
	priv->first_blockgroup =
		superblock->superblock_id +
		(sizeof(struct ext2_superblock) / priv->block_size);
	priv->blockgroup_count =
		((superblock->block_count - superblock->superblock_id +
		  (superblock->blocks_per_group - 1)) /
		 superblock->blocks_per_group);

	ext2_read_inode(dev, priv, 11);

	return 0;
}
