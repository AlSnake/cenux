#include <fs/ext2/inode.h>
#include <kmem.h>

static void ext2_read_block(uint8_t* buf, uint32_t block, struct ext2_priv* priv)
{
	uint32_t spb = priv->sectors_per_block;
	if(!spb) spb = 1;
	priv->dev->read(priv->dev, spb * block, buf, spb);
}

int32_t ext2_inode_read(uint32_t inode, struct ext2_inode* inode_buf, struct ext2_priv* priv)
{
	uint32_t bg = (inode - 1) / priv->sb->inodes_per_group;
	
	uint8_t *buf = kmalloc(priv->block_size);
	if(!buf) return -1;

	ext2_read_block(buf, priv->first_bg, priv);

	return 0;
}
