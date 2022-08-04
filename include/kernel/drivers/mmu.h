#include <types.h>

#ifndef MMU_H
#define MMU_H

#define KERNEL_VA_BEGIN 0xFFFF000000000000
#define PHYS_MEM_SIZE   0x0000000040000000
#define PAGE_MASK       0xFFFFFFFFFFFFF000
#define DEVICE_BASE     0x3F000000

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

#define MMU_MAIR        0

#define MMU_SCTLR_ENABLED   (1)

#endif