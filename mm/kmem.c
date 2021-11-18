#include <mm/kmem.h>
#include <mm/heap.h>
#include <kernel/kprintf.h>
#include <cenux/string.h>

struct heap kheap;
struct block_table kblock_table;

void kmem_init()
{
	int total_blocks = HEAP_SIZE_BYTES / HEAP_BLOCK_SIZE;

	kblock_table.blocks = (uint8_t *)HEAP_BLOCK_TABLE_ADDRESS;
	kblock_table.size = total_blocks;

	void *heap_end = (void *)(HEAP_ADDRESS + HEAP_SIZE_BYTES);

	if (heap_create(&kheap, (void *)HEAP_ADDRESS, heap_end, &kblock_table) <
	    0)
		kprintf("FAILED TO CREATE HEAP\n");
}

void *kmalloc(size_t size)
{
	return heap_malloc(&kheap, size);
}

void *kzalloc(size_t size)
{
	void *ptr = kmalloc(size);
	if (!ptr)
		return 0;

	memset(ptr, 0x00, size);
	return ptr;
}

void kfree(void *ptr)
{
	heap_free(&kheap, ptr);
}
