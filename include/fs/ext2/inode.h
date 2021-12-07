#ifndef _FS_EXT2_INODE_H_
#define _FS_EXT2_INODE_H_

#include <stdint.h>
#include <fs/ext2/ext2.h>

/* INODE TYPES */
#define EXT2_S_IFIFO	0x1000
#define EXT2_S_IFCHR	0x2000
#define EXT2_S_IFDIR	0x4000
#define EXT2_S_IFBLK	0x6000
#define EXT2_S_IFREG	0x8000
#define EXT2_S_IFLNK	0xA000
#define EXT2_S_IFSOCK	0xC000

/* INODE PERMISSIONS */
#define EXT2_S_IXOTH	0x0001
#define EXT2_S_IWOTH	0x0002
#define EXT2_S_IROTH	0x0004
#define EXT2_S_IXGRP	0x0008
#define EXT2_S_IWGRP	0x0010
#define EXT2_S_IRGRP	0x0020
#define EXT2_S_IXUSR	0x0040
#define EXT2_S_IWUSR	0x0080
#define EXT2_S_IRUSR	0x0100
#define EXT2_S_ISVTX	0x0200
#define EXT2_S_ISGID	0x0400
#define EXT2_S_ISUID	0x0800

/* EXT2 INODE */
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
}__attribute__((packed));

/* READ INODE */
int32_t ext2_inode_read(uint32_t inode, struct ext2_inode* inode_buf, struct ext2_priv* priv);

#endif
