#ifndef _MM_HEAP_H_
#define _MM_HEAP_H_

#include <stdint.h>
#include <stddef.h>

#define HEAP_BLOCK_TAKEN 		0x01
#define HEAP_BLOCK_FREE 		0x00

#define HEAP_BLOCK_START 		0x80 		
#define HEAP_BLOCK_CONTIGUOUS 		0x40

#define HEAP_BLOCK_SIZE 		4096
#define HEAP_SIZE_BYTES 		104857600 	/* 100MB */ 
#define HEAP_ADDRESS 			0x01000000
#define HEAP_BLOCK_TABLE_ADDRESS 	0x00007E00

#define HEAP_IS_ALIGNED(addr)		((uint32_t)addr % HEAP_BLOCK_SIZE) == 0
#define HEAP_GET_BLOCK_TYPE(block)	(block & 0x0f)
#define HEAP_ALIGN_BLOCK_SIZE(size)	(size % HEAP_BLOCK_SIZE ? (size - (size % HEAP_BLOCK_SIZE)) + HEAP_BLOCK_SIZE : size)

struct block_table {
	uint8_t* blocks; /* HEAP_BLOCK_START, HEAP_BLOCK_CONTIGUOUS, 0, 0, HEAP_BLOCK_TYPE_4BIT */
	size_t size;
};

struct heap {
	struct block_table* block_table;
	void* start_addr;
};

int32_t heap_create(struct heap* heap, void* heap_start, void* heap_end, struct block_table* block_table);
void* heap_malloc(struct heap* heap, size_t size);
void heap_free(struct heap* heap, void* ptr);

#endif
