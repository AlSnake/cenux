#include <fs/fstream.h>
#include <mm/kmem.h>
#include <kernel/panic.h>

#define SECTOR_SIZE 512

struct fstream *fstream_init(struct device *dev)
{
	struct fstream *stream = kzalloc(sizeof(struct fstream));
	if (!stream)
		panic("NOMEM");
	stream->pos = 0;
	stream->dev = dev;
	return stream;
}

void fstream_seek(struct fstream *stream, int32_t pos)
{
	stream->pos = pos;
}

int32_t fstream_read(struct fstream *stream, void *buf, uint32_t len)
{
	struct device *dev = stream->dev;
	uint32_t sector = stream->pos / SECTOR_SIZE;
	uint32_t offset = stream->pos % SECTOR_SIZE;
	uint32_t resize = len > SECTOR_SIZE ? SECTOR_SIZE : len;
	uint8_t diskbuf[SECTOR_SIZE];
	int32_t ret = 0;

	if (dev->read(dev, sector, diskbuf, 1) < 0)
		return -1;

	for (uint32_t i = 0; i < resize; i++)
		*(uint8_t *)buf++ = diskbuf[i + offset];

	stream->pos = resize;
	if (len > SECTOR_SIZE)
		ret = fstream_read(stream, buf, len - SECTOR_SIZE);

	return ret;
}

void fstream_close(struct fstream *stream)
{
	kfree(stream);
}
