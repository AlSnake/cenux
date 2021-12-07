#ifndef _FS_EXT2_SUPERBLOCK_H_
#define _FS_EXT2_SUPERBLOCK_H_

#include <stdint.h>
#include <drivers/device.h>
#include <stddef.h>

/* EXT2 FLAGS */
#define EXT2_SIGNATURE 0xEF53

/* REQUIRED FEATURE FLAGS */
#define EXT2_DIRENT_TYPE_FIELD 0x2

/* READ-ONLY FEATURE FLAGS */
#define EXT2_SPARSE_SUPERBLOCKS 0x1

/* EXT2 SUPERBLOCK */
struct ext2_superblock {
	uint32_t inode_count; // TOTAL NUMBER OF INODES
	uint32_t block_count; // TOTAL NUMBER OF BLOCKS
	uint32_t reserved_blocks; // RESERVED BLOCKS FOR SUPERUSER
	uint32_t unallocated_blocks; // NUMBER OF UNALLOCATED BLOCKS
	uint32_t unallocated_inodes; // NUMBER OF UNALLOCATED INODES
	uint32_t superblock_id; // BLOCK NUMBER THAT CONTAINS SUPERBLOCK (FIRST BLOCK)
	uint32_t block_size; // BLOCK SIZE (shift left by 1024)
	uint32_t fragment_size; // FRAGMENT SIZE (shift left by 1024)
	uint32_t blocks_per_group; // NUMBER OF BLOCKS PER GROUP
	uint32_t fragments_per_group; // NUMBER OF FRAGMENTS PER GROUP
	uint32_t inodes_per_group; // NUMBER OF INODES PER GROUP
	uint32_t mount_time; // LAST MOUNT TIME
	uint32_t write_time; // LAST WRITE TIME
	uint16_t mount_count; // NUMBER OF MOUNTS SINCE LAST FSCK
	uint16_t max_mount; // NUMBER OF MAX MOUNTS BEFORE FSCK
	uint16_t signature; // EXT2 SIGNATURE
	uint16_t status; // FILE SYSTEM STATUS
	uint16_t error; // ERROR HANDLER
	uint16_t minor; // MINOR VERSION
	uint32_t last_fsck; // LAST FSCK TIME
	uint32_t fsck_interval; // INTERVAL BETWEEN FORCED FSCK
	uint32_t os_id; // OPERATING SYSTEM ID
	uint32_t major; // MAJOR VERSION
	uint16_t uid; // USERID THAT CAN USE RESERVED BLOCKS
	uint16_t gid; // GROUPID THAT CAN USE RESERVED BLOCKS

	/* Extended Superblock Fields - MAJOR >= 1 */
	uint32_t inode; // FIRST USUABLE INODE (In versions < 1.0, this is fixed as 11)
	uint16_t inode_size; // SIZE OF EACH INODE (In versions < 1.0, this is fixed as 128)
	uint16_t blockgroup; // BLOCK GROUP IN WHICH THIS SUPERBLOCK IS LOCATED
	uint32_t optional_features; // OPTIONAL FEATURES (NOT REQUIRED TO SUPPORT R/W BUT INCREASES PERFORMANCE)
	uint32_t required_features; // REQUIRED FEATURES (REQUIRED TO SUPPORT R/W)
	uint32_t readonly_features; // FEATURES THAT IF NOT SUPPORTED, VOLUME MUST BE MOUNTED READ-ONLY
	uint32_t fs_id[4]; // FILE SYSTEM ID
	uint8_t volume_name[16]; // VOLUME NAME (MUST BE NULL TERMINATED)
	uint8_t last_mounted[64]; // LAST MOUNTED VOLUME PATH (MUST BE NULL TERMINATED)
	uint32_t compression_algo; // COMPRESSION ALGORITHMS USED
	uint8_t preallocate_files; // NUMBER OF BLOCKS TO PREALLOCATE FOR FILES
	uint8_t preallocate_dirs; // NUMBER OF BLOCKS TO PREALLOCATE FOR DIRECTORIES
	uint16_t _unused1; // UNUSED BYTES
	uint32_t journal_id[4]; // JOURNAL ID
	uint32_t journal_inode; // JOURNAL INODE
	uint32_t journal_device; // JOURNAL DEVICE
	uint32_t orphan_inode_head; // HEAD OF ORPHAN INODE LIST
	uint8_t _unused[788];
}__attribute__((packed));

/* EXTRACT SUPERBLOCK */
int32_t ext2_superblock_read(struct device* dev, struct ext2_superblock* sb);

/* PRINT SUPERBLOCK DATA */
void ext2_superblock_dump(struct ext2_superblock* sb);

/* GET BLOCK SIZE */
size_t ext2_superblock_blocksize(struct ext2_superblock* sb);

/* GET NUMBER OF BLOCK GROUPS */
size_t ext2_superblock_blockgroups(struct ext2_superblock* sb);

/* GET INODE SIZE */
size_t ext2_superblock_inodesize(struct ext2_superblock* sb);

#endif
