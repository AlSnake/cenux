#ifndef _MM_PAGING_H_
#define _MM_PAGING_H_

#include <stdint.h>
#include <stddef.h>

#define PAGING_IS_PRESENT	0x1
#define PAGING_IS_WRITEABLE	0x2
#define PAGING_ACCESS_FROM_ALL	0x4
#define PAGING_WRITE_THROUGH	0x8
#define PAGING_CACHE_DISABLED	0x10

#define PAGING_TOTAL_ENTRIES	1024
#define PAGING_PAGE_SIZE	4096

#define PAGING_IS_ALIGNED(addr)	((uint32_t)addr % PAGING_PAGE_SIZE) == 0
#define PAGING_DIR_INDEX(vaddr)	((uint32_t)vaddr / (PAGING_TOTAL_ENTRIES * PAGING_PAGE_SIZE))
#define PAGING_TBL_INDEX(vaddr) ((uint32_t)vaddr % (PAGING_TOTAL_ENTRIES * PAGING_PAGE_SIZE) / PAGING_PAGE_SIZE)
#define PAGING_ALIGN_LOWER(addr)((addr - (uint32_t)addr % PAGING_PAGE_SIZE))

struct page_directory
{
	uint32_t* directory_entry;
};

extern void enable_paging();
struct page_directory* paging_alloc(uint8_t flags);
void paging_free(struct page_directory* dir);
void paging_switch(struct page_directory* dir);
int32_t paging_map(struct page_directory* dir, void* virt, void* phys, uint8_t flags);
int32_t paging_map_range(struct page_directory* dir, void* virt, void* phys_start, uint32_t range, uint8_t flags);
int32_t paging_map_region(struct page_directory* dir, void* virt, void* phys_start, void* phys_end, uint8_t flags);
uint32_t paging_get_table(struct page_directory* dir, void* virt);
void* virt_to_phys(struct page_directory* dir, void* virt);

#endif
