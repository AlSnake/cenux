#ifndef _FS_EXT2_H_
#define _FS_EXT2_H_

#include <fs/vfs.h>
#include <fs/ext2/superblock.h>

struct filesystem* ext2_init(struct device* dev);

struct ext2_priv {
	struct device* dev;
	struct ext2_superblock* sb;
	uint32_t block_size;
	uint32_t inode_size;
	uint32_t inodes_per_block;
	uint32_t sectors_per_block;
	uint32_t first_bg;
	uint32_t blockgroup_count;
}__attribute__((packed));

struct ext2_priv* ext2_parse_superblock(struct ext2_superblock* sb);

#endif
