#ifndef _FS_FAT_PARSER_H_
#define _FS_FAT_PARSER_H_

#include <stdint.h>

struct fat_drive {
	char label;
	struct fat_directory* root;
};

struct fat_directory {
	const char* name;
	struct fat_directory* next;
};

struct fat_drive* fat_parser_parse(const char* path);
void fat_parser_free(struct fat_drive* drive);

#endif
