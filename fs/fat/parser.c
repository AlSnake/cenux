#include <fs/fat/parser.h>
#include <cenux/limits.h>
#include <cenux/string.h>
#include <cenux/ctype.h>
#include <mm/kmem.h>

#define FAT_PATH_MAX 255

static int32_t validate_path(const char *path)
{
	size_t len = strnlen(path, FAT_PATH_MAX);
	return (len >= 3 && isalpha(path[0]) &&
		!memcmp((void *)&path[1], ":/", 2));
}

static char get_label(const char *path)
{
	if (!validate_path(path))
		return -1;

	return path[0];
}

static char *get_path(const char *path)
{
	path += 3;
	return (char *)path;
}

static char **get_directories(const char *path)
{
	char *tmp = 0;
	char *token = 0;
	char **tokens = 0;
	char *saveptr = 0;
	size_t i = 0;

	tmp = strdup(path);
	if (!tmp)
		return 0;

	tokens = kmalloc((FAT_PATH_MAX / 3) * sizeof(char *));
	if (!tokens) {
		kfree(tmp);
		return 0;
	}

	token = strtok_r(tmp, "/", &saveptr);
	while (token) {
		tokens[i++] = token;
		token = strtok_r(0, "/", &saveptr);
	}

	tokens[i] = NULL;
	return tokens;
}

static struct fat_drive *init_drive(char label)
{
	struct fat_drive *drive = kzalloc(sizeof(struct fat_drive));
	drive->label = label;
	drive->root = 0;
	return drive;
}

static struct fat_directory *init_directory(const char *path)
{
	struct fat_directory *dir = kzalloc(sizeof(struct fat_directory));
	if (!dir)
		return 0;

	char **dirs = get_directories(path);
	if (!dirs) {
		kfree(dir);
		return 0;
	}

	struct fat_directory *cdir = dir;
	for (size_t i = 0; dirs[i] != NULL; i++) {
		cdir->name = dirs[i];
		cdir->next = kzalloc(sizeof(struct fat_directory));
		cdir = cdir->next;
	}
	kfree(cdir);
	cdir = 0;

	return dir;
}

struct fat_drive *fat_parser_parse(const char *path)
{
	struct fat_drive *drive;

	if (strlen(path) > FAT_PATH_MAX)
		return 0;

	char label = get_label(path);
	if (label == -1)
		return 0;

	char *jpath = get_path(path);

	drive = init_drive(label);
	if (!drive)
		return 0;

	struct fat_directory *dir = init_directory(jpath);
	if (!dir)
		return 0;

	drive->root = dir;

	return drive;
}

void fat_parser_free(struct fat_drive *drive)
{
	struct fat_directory *cdir = drive->root;
	while (cdir) {
		struct fat_directory *ndir = cdir->next;
		kfree((void *)cdir->name);
		kfree(cdir);
		cdir = ndir;
	}

	kfree(drive);
}
