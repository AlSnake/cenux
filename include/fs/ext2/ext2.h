#ifndef _FS_EXT2_H_
#define _FS_EXT2_H_

#include <drivers/device.h>

int32_t ext2_init(struct device *dev);

struct ext2_superblock {
	uint32_t inode_count; // TOTAL NUMBER OF INODES
	uint32_t block_count; // TOTAL NUMBER OF BLOCKS
	uint32_t reserved_blocks; // RESERVED BLOCKS FOR SUPERUSER
	uint32_t unallocated_blocks; // NUMBER OF UNALLOCATED BLOCKS
	uint32_t unallocated_inodes; // NUMBER OF UNALLOCATED INODES
	uint32_t superblock_id; // BLOCK NUMBER THAT CONTAINS SUPERBLOCK
	uint32_t block_size; // BLOCK SIZE
	uint32_t fragment_size; // FRAGMENT SIZE
	uint32_t blocks_per_group; // NUMBER OF BLOCKS PER GROUP
	uint32_t fragments_per_group; // NUMBER OF FRAGMENTS PER GROUP
	uint32_t inodes_per_group; // NUMBER OF INODES PER GROUP
	uint32_t mount_time; // LAST MOUNT TIME
	uint32_t write_time; // LAST WRITE TIME
	uint16_t mount_count; // NUMBER OF MOUNTS SINCE LAST FSCK
	uint16_t max_mount; // NUMBER OF MAX MOUNTS BEFORE FSCK IS RUN
	uint16_t signature; // EXT2 SIGNATURE
	uint16_t state; // FILE SYSTEM STATE
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
	uint8_t last_mounted
		[64]; // LAST MOUNTED VOLUME PATH (MUST BE NULL TERMINATED)
	uint32_t compression_algo; // COMPRESSION ALGORITHMS USED
	uint8_t preallocate_files; // NUMBER OF BLOCKS TO PREALLOCATE FOR FILES
	uint8_t preallocate_dirs; // NUMBER OF BLOCKS TO PREALLOCATE FOR DIRECTORIES
	uint16_t _unused1; // UNUSED BYTES
	uint32_t journal_id[4]; // JOURNAL ID
	uint32_t journal_inode; // JOURNAL INODE
	uint32_t journal_device; // JOURNAL DEVICE
	uint32_t orphan_inode_head; // HEAD OF ORPHAN INODE LIST
	uint8_t _unused[788];
} __attribute__((packed));

struct ext2_blockgroup_desc {
	uint32_t blocks_bitmap; // BLOCK NUMBER OF BLOCK BITMAP, USED FOR BLOCK (DE)ALLOCATION
	uint32_t inodes_bitmap; // BLOCK NUMBER OF INODE BITMAP, USED FOR INODE (DE)ALLOCATION
	uint32_t inode_table; // BLOCK NUMBER OF STARTING BLOCK FOR INODE TABLE
	uint16_t unallocated_blocks; // NUMBER OF UNALLOCATED BLOCKS
	uint16_t unallocated_inodes; // NUMBER OF UNALLOCATED INODES
	uint16_t dir_count; // NUMBER OF DIRECTORIES
	uint8_t _unused[14];
} __attribute__((packed));

struct ext2_inode {
	uint16_t mode; // TYPE AND PERMISSION
	uint16_t uid; // USER ID
	uint32_t size_lo; // LOWER 32bits of SIZE
	uint32_t atime; // LAST ACCESS TIME
	uint32_t ctime; // CREATION TIME
	uint32_t mtime; // LAST MODIFICATION TIME
	uint32_t dtime; // DELETION TIME
	uint16_t gid; // GROUP ID
	uint16_t hard_links; // NUMBER OF HARD LINKS (0 = BLOCK IS MARKED AS UNALLOCATED)
	uint32_t sector_count; // NUMBER OF DISK SECTORS IN USE BY THIS INODE
	uint32_t flags; // FLAGS
	uint32_t ossv1; // OPERATING SYSTEM SPECIFIC VALUE #1
	uint32_t dbp[12]; // DIRECT BLOCK POINTERS
	uint32_t sibp; // SINGLY INDIRECT BLOCK POINTER
	uint32_t dibp; // DOUBLY INDIRECT BLOCK POINTER
	uint32_t tibp; // TRIPLY INDIRECT BLOCK POINTER
	uint32_t generation; // GENERATION NUMBER
	union {
		uint32_t reserved1;
		uint32_t file_acl;
	};
	union {
		uint32_t reserved2;
		uint32_t size_hi;
		uint32_t dir_acl;
	};
	uint32_t fragment_addr; // BLOCK ADDRESS OF FRAGMENT
	uint8_t ossv2[12]; // OPERATING SYSTEM SPECIFIC VALUE #2
} __attribute__((packed));

struct ext2_priv {
	struct device *dev;
	struct ext2_superblock *superblock;
	uint32_t block_size;
	uint32_t inode_size;
	uint32_t inodes_per_group;
	uint32_t first_blockgroup;
	uint32_t blockgroup_count;
	uint32_t superblock_id;
} __attribute__((packed));

#endif
