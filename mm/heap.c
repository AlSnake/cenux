#include <mm/heap.h>
#include <cenux/errno.h>
#include <cenux/string.h>

/* Check if heap_start and heap_end is valid for block table */
static int32_t bad_block_table(void *heap_start, void *heap_end,
			       struct block_table *block_table)
{
	size_t table_size = (size_t)(heap_end - heap_start);
	size_t total_blocks = table_size / HEAP_BLOCK_SIZE;

	if (block_table->size != total_blocks)
		return -EINVAL;

	return 0;
}
static int32_t get_start_block(struct heap *heap, uint32_t total_blocks)
{
	struct block_table *block_table = heap->block_table;
	int32_t start_block = -1;
	uint32_t current_block = 0;

	for (size_t i = 0; i < block_table->size; i++) {
		if (HEAP_GET_BLOCK_TYPE(block_table->blocks[i]) !=
		    HEAP_BLOCK_FREE) {
			start_block = -1;
			current_block = 0;
			continue;
		}

		if (start_block == -1)
			start_block = i;

		current_block++;

		if (current_block == total_blocks)
			break;
	}

	if (start_block == -1)
		return -ENOMEM;

	return start_block;
}

static void *block_to_addr(struct heap *heap, uint32_t block)
{
	return heap->start_addr + (block * HEAP_BLOCK_SIZE);
}

static uint32_t addr_to_block(struct heap *heap, void *addr)
{
	return ((uint32_t)(addr - heap->start_addr)) / HEAP_BLOCK_SIZE;
}

static void set_blocks_taken(struct heap *heap, uint32_t start_block,
			     uint32_t total_blocks)
{
	uint32_t end_block = (start_block + total_blocks) - 1;

	uint8_t block = HEAP_BLOCK_TAKEN | HEAP_BLOCK_START;

	if (total_blocks > 1)
		block |= HEAP_BLOCK_CONTIGUOUS;

	for (uint32_t i = start_block; i <= end_block; i++) {
		heap->block_table->blocks[i] = block;
		block = HEAP_BLOCK_TAKEN;
		if (i != end_block - 1)
			block |= HEAP_BLOCK_CONTIGUOUS;
	}
}

static void set_blocks_free(struct heap *heap, uint32_t start_block)
{
	struct block_table *block_table = heap->block_table;

	for (uint32_t i = start_block; i < block_table->size; i++) {
		uint8_t block = block_table->blocks[i];
		block_table->blocks[i] = HEAP_BLOCK_FREE;
		if (!(block & HEAP_BLOCK_CONTIGUOUS))
			break;
	}
}

static void *malloc_blocks(struct heap *heap, uint32_t total_blocks)
{
	void *addr = 0;

	int32_t start_block = get_start_block(heap, total_blocks);
	if (start_block < 0)
		return addr;

	addr = block_to_addr(heap, start_block);

	set_blocks_taken(heap, start_block, total_blocks);

	return addr;
}

int32_t heap_create(struct heap *heap, void *heap_start, void *heap_end,
		    struct block_table *block_table)
{
	if (!HEAP_IS_ALIGNED(heap_start) || !HEAP_IS_ALIGNED(heap_end))
		return -EINVAL;

	memset(heap, 0, sizeof(struct heap));
	heap->start_addr = heap_start;
	heap->block_table = block_table;

	if (bad_block_table(heap_start, heap_end, block_table))
		return -EINVAL;

	size_t table_size = sizeof(uint8_t) * block_table->size;
	memset(block_table->blocks, HEAP_BLOCK_FREE, table_size);

	return 0;
}

void *heap_malloc(struct heap *heap, size_t size)
{
	size_t aligned_size = HEAP_ALIGN_BLOCK_SIZE(size);
	uint32_t total_blocks = aligned_size / HEAP_BLOCK_SIZE;
	return malloc_blocks(heap, total_blocks);
}

void heap_free(struct heap *heap, void *ptr)
{
	set_blocks_free(heap, addr_to_block(heap, ptr));
}
