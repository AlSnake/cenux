#include <fs/ext2/ext2.h>
#include <mm/kmem.h>
#include <kernel/kprintf.h>
#include <cenux/string.h>
#include <fs/ext2/superblock.h>

static int32_t ext2_read(struct device* dev, uint32_t offset, uint8_t* buf, uint32_t len)
{
	if(!dev->read) {
		kprintf("EXT2: NO READ OPERATION\n");
		return -1;
	}

	return 0;
}

struct filesystem* ext2_init(struct device* dev)
{
	if(!dev->read) {
		kprintf("EXT2: NO READ OPERATION\n");
		return 0;
	}

	struct ext2_superblock* sb = kmalloc(sizeof(struct ext2_superblock));
	if(!sb) return 0;

	if(ext2_superblock_read(dev, sb) == -1) {
		kprintf("EXT2: INVALID DRIVE\n");
		kfree(sb);
		return 0;
	}

	struct filesystem* fs = kmalloc(sizeof(struct filesystem));
	if(!fs) {
		kfree(sb);
		return 0;
	}

	kprintf("VALID EXT2 SIGNATURE: %x\n", sb->signature);
	ext2_superblock_dump(sb);

	struct ext2_priv* ext2priv = ext2_parse_superblock(sb);
	if(!ext2priv) {
		kprintf("EXT2: COULDN'T PARSE SUPERBLOCK\n");
		kfree(sb);
		kfree(fs);
		return 0;
	}

	ext2priv->dev = dev;
	
	fs->name = "EXT2";
	fs->dev = dev;
	fs->read = ext2_read;
	fs->priv = ext2priv;
	return fs;
}

struct ext2_priv* ext2_parse_superblock(struct ext2_superblock* sb)
{
	struct ext2_priv* ext2priv = kmalloc(sizeof(struct ext2_priv));
	if(!ext2priv)
		return 0;
	
	ext2priv->sb = sb;
	ext2priv->block_size = ext2_superblock_blocksize(sb);
	ext2priv->first_bg = sb->superblock_id + (sizeof(struct ext2_superblock) / ext2priv->block_size);

	return ext2priv;
}
