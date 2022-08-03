#include <types.h>
#include <drivers/mmio.h>

#ifndef MM_H
#define MM_H

#define MM_PAGE_SHIFT       12
#define MM_TABLE_SHIFT      9
#define MM_SECTION_SHIFT    (MM_PAGE_SHIFT + MM_TABLE_SHIFT)
#define MM_PAGE_SIZE        (1 << MM_PAGE_SHIFT)
#define MM_SECTION_SIZE     (1 << MM_SECTION_SHIFT)

//#define MM_MEMORY_LOW       (2 * MM_SECTION_SIZE)
#define MM_MEMORY_LOW       0xF0000000
#define MM_MEMORY_HIGH      PERIPHERAL_BASE

#define MM_PAGING_MEMORY    (MM_MEMORY_HIGH - MM_MEMORY_LOW)
#define MM_PAGING_PAGES     (MM_PAGING_MEMORY / MM_PAGE_SIZE)

uint64_t mm_get_free_page();
void mm_free_page(uint64_t ptr);
void mm_memzero(uint64_t src, uint64_t n);

#endif