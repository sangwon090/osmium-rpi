#ifndef MMU_H
#define MMU_H

#include <types.h>
#include <drivers/mmio.h>

#define VA_START            0xFFFF000000000000

#define PHYS_MEMORY_SIZE    0x40000000

#define PAGE_MASK           0xFFFFFFFFFFFFF000
#define PAGE_SHIFT          12
#define TABLE_SHIFT         9
#define SECTION_SHIFT       (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE           (1 << PAGE_SHIFT)    
#define SECTION_SIZE        (1 << SECTION_SHIFT)    

#define LOW_MEMORY          (SECTION_SIZE * 2)
#define HIGH_MEMORY         PERIPHERAL_BASE

#define PAGING_MEMORY       (HIGH_MEMORY - LOW_MEMORY)
#define PAGING_PAGES        (PAGING_MEMORY / PAGE_SIZE)

#define PTRS_PER_TABLE      (1 << TABLE_SHIFT)

#define PGD_SHIFT           PAGE_SHIFT + TABLE_SHIFT * 3
#define PUD_SHIFT           PAGE_SHIFT + TABLE_SHIFT * 2
#define PMD_SHIFT           PAGE_SHIFT + TABLE_SHIFT

#define PG_DIR_SIZE         (PAGE_SIZE * 3)

// Execute Never
#define MMU_UXN_OFF (0 << 54)
#define MMU_UXN_ON  (1 << 54)
#define MMU_PXN_OFF (0 << 53)
#define MMU_PXN_ON  (1 << 53)

// Access Flag
#define MMU_AF_NOT_ACCESSED (0 << 10)
#define MMU_AF_ACCESSED     (1 << 10)

// Shareable Attribute
#define MMU_SH_NON_SHAREABLE    (0 << 8)
#define MMU_SH_OUTER_SHAREABLE  (2 << 8)
#define MMU_SH_INNER_SHAREABLE  (3 << 8)

// Access Permission
#define MMU_AP_RW       (0 << 7)
#define MMU_AP_RO       (1 << 7)
#define MMU_AP_USER     (0 << 6)
#define MMU_AP_KERNEL   (1 << 6)

// MAIR
#define MMU_MAIR_MEMORY         (0 << 2)
#define MMU_MAIR_DEVICE         (1 << 2)
#define MMU_MAIR_NON_CACHEABLE  (2 << 2)

#define MMU_BLOCK   1
#define MMU_TABLE   3

#define MMU_FLAGS_MEMORY    (MMU_BLOCK | MMU_MAIR_NON_CACHEABLE | MMU_AF_ACCESSED)
#define MMU_FLAGS_DEVICE    (MMU_BLOCK | MMU_MAIR_DEVICE        | MMU_AF_ACCESSED)
#define MMU_FLAGS_PTABLE    (MMU_TABLE | MMU_MAIR_NON_CACHEABLE | MMU_AF_ACCESSED | MMU_AP_KERNEL)

#define MMU_TCR_T0SZ    (16)
#define MMU_TCR_T1SZ    (16 << 16)
#define MMU_TCR_TG0_4K  (0 << 14)
#define MMU_TCR_TG1_4K  (2 << 30)
#define MMU_TCR         (MMU_TCR_T0SZ | MMU_TCR_T1SZ | MMU_TCR_TG0_4K | MMU_TCR_TG1_4K)

#define MMU_T0SZ        
#define MMU_MAIR        0

#define MMU_SCTLR_ENABLED   (1)      

#ifndef __ASSEMBLER__

extern uint64_t pg_dir;

void mmu_init();
void mmu_create_table_entry(uint64_t table_addr, uint64_t table_next_addr, uint64_t virt_addr, uint64_t shift, uint64_t flags);
void mmu_create_block_entry(uint64_t table_addr, uint64_t phys_addr, uint64_t begin, uint64_t end, uint64_t flags);

#endif

#endif