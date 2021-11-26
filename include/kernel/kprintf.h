#ifndef _KERNEL_KPRINTF_H_
#define _KERNEL_KPRINTF_H_

#include <stdint.h>
#include <stdarg.h>

int32_t kprintf(const char* fmt, ...);

#endif
