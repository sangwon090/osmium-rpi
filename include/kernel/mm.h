#include <types.h>
#include <drivers/mmio.h>

#ifndef MM_H
#define MM_H

#define KERNEL_VA_BEGIN     0xFFFF000000000000
#define PHYSICAL_MEM_SIZE   0x0000000040000000
#define PAGE_MASK           0xFFFFFFFFFFFFF000

#define PAGE_SHIFT       12
#define TABLE_SHIFT      9
#define SECTION_SHIFT    (PAGE_SHIFT + TABLE_SHIFT)
#define PAGE_SIZE        (1 << PAGE_SHIFT)
#define SECTION_SIZE     (1 << SECTION_SHIFT)

#define MEMORY_LOW       (2 * SECTION_SIZE)
#define MEMORY_HIGH      PERIPHERAL_BASE

#define PAGING_MEMORY    (MEMORY_HIGH - MEMORY_LOW)
#define PAGING_PAGES     (PAGING_MEMORY / PAGE_SIZE)

#define PTRS_PER_TABLE   (1 << TABLE_SHIFT)
#define PGD_SHIFT           PAGE_SHIFT + TABLE_SHIFT * 3
#define PUD_SHIFT           PAGE_SHIFT + TABLE_SHIFT * 2
#define PMD_SHIFT           PAGE_SHIFT + TABLE_SHIFT
#define PG_DIR_SIZE         (PAGE_SIZE * 3)

#ifndef __ASSEMBLER__

uint64_t mm_get_free_page();
void mm_free_page(uint64_t ptr);
void mm_memzero(uint64_t src, uint64_t n);

#endif

#endif