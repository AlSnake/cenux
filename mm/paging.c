#include <mm/paging.h>
#include <mm/kmem.h>
#include <cenux/errno.h>

static struct page_directory *current_directory = 0;

extern void paging_load_directory(uint32_t *directory);

struct page_directory *paging_alloc(uint8_t flags)
{
	struct page_directory *page = kzalloc(sizeof(struct page_directory));
	uint32_t *directory = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES);
	uint32_t offset = 0;

	for (uint32_t i = 0; i < PAGING_TOTAL_ENTRIES; i++) {
		uint32_t *table_entry =
			kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES);

		for (uint32_t j = 0; j < PAGING_TOTAL_ENTRIES; j++)
			table_entry[j] =
				(offset + (j * PAGING_PAGE_SIZE)) | flags;

		offset += (PAGING_TOTAL_ENTRIES * PAGING_PAGE_SIZE);
		directory[i] =
			(uint32_t)table_entry | flags | PAGING_IS_WRITEABLE;
	}

	page->directory_entry = directory;
	return page;
}

void paging_free(struct page_directory *dir)
{
	for (uint32_t i = 0; i < PAGING_TOTAL_ENTRIES; i++) {
		uint32_t entry = dir->directory_entry[i];
		uint32_t *table = (uint32_t *)(entry & 0xfffff000);
		kfree(table);
	}

	kfree(dir->directory_entry);
	kfree(dir);
}

void paging_switch(struct page_directory *dir)
{
	paging_load_directory(dir->directory_entry);
	current_directory = dir;
}

int32_t paging_map(struct page_directory *dir, void *virt, void *phys,
		   uint8_t flags)
{
	if (!PAGING_IS_ALIGNED(virt))
		return -EINVAL;

	uint32_t dir_index = PAGING_DIR_INDEX(virt);
	uint32_t tbl_index = PAGING_TBL_INDEX(virt);

	uint32_t entry = dir->directory_entry[dir_index];
	uint32_t *table = (uint32_t *)(entry & 0xfffff000);
	table[tbl_index] = (uint32_t)phys | flags;

	return 0;
}

int32_t paging_map_range(struct page_directory *dir, void *virt, void *phys,
			 uint32_t range, uint8_t flags)
{
	int ret = 0;
	for (uint32_t i = 0; i < range; i++) {
		ret = paging_map(dir, virt, phys, flags);
		if (ret < 0)
			break;

		virt += PAGING_PAGE_SIZE;
		phys += PAGING_PAGE_SIZE;
	}

	return ret;
}

int32_t paging_map_region(struct page_directory *dir, void *virt,
			  void *phys_start, void *phys_end, uint8_t flags)
{
	if (!PAGING_IS_ALIGNED(virt) || !PAGING_IS_ALIGNED(phys_start) ||
	    !PAGING_IS_ALIGNED(phys_end))
		return -EINVAL;

	if ((uint32_t)phys_end < (uint32_t)phys_start)
		return -EINVAL;

	uint32_t range = phys_end - phys_start;
	return paging_map_range(dir, virt, phys_start, range, flags);
}

uint32_t paging_get_table(struct page_directory *dir, void *virt)
{
	uint32_t dir_index = PAGING_DIR_INDEX(virt);
	uint32_t tbl_index = PAGING_TBL_INDEX(virt);
	uint32_t entry = dir->directory_entry[dir_index];
	uint32_t *table = (uint32_t *)(entry & 0xfffff000);
	return table[tbl_index];
}

void *virt_to_phys(struct page_directory *dir, void *virt)
{
	void *align_addr = PAGING_ALIGN_LOWER(virt);
	void *differ = (void *)((uint32_t)virt - (uint32_t)align_addr);
	uint32_t table = paging_get_table(dir, virt);
	return ((table & 0xfffff000) + differ);
}
