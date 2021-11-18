#ifndef _MM_KMEM_H_
#define _MM_KMEM_H_

#include <stdint.h>
#include <stddef.h>

void kmem_init();
void* kmalloc(size_t size);
void* kzalloc(size_t size);
void kfree(void* ptr);

#endif
