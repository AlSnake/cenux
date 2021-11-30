#ifndef _FS_FSTREAM_H_
#define _FS_FSTREAM_H_

#include <drivers/device.h>
#include <stdint.h>

struct fstream {
	int32_t pos;
	struct device* dev;
};

struct fstream* fstream_init(struct device* dev);
void fstream_seek(struct fstream* stream, int32_t pos);
int32_t fstream_read(struct fstream* stream, void* buf, uint32_t len);
void fstream_close(struct fstream* stream);

#endif
