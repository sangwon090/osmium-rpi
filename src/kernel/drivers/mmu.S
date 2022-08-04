#include <drivers/mmio.h>
#include <drivers/mmu.h>
#include <mm.h>

.macro table_entry, tbl, virt, shift, tmp1, tmp2
    lsr \tmp1, \virt, #\shift
    and \tmp1, \tmp1, #PTRS_PER_TABLE - 1
    add \tmp2, \tbl, #PAGE_SIZE
    orr \tmp2, \tmp2, #MMU_TABLE
    str \tmp2, [\tbl, \tmp1, lsl #3]
    add \tbl, \tbl, #PAGE_SIZE
.endm

.macro block_entry, tbl, phys, start, end, flags, tmp1
    lsr    \start, \start, #SECTION_SHIFT
    and    \start, \start, #PTRS_PER_TABLE - 1
    lsr    \end, \end, #SECTION_SHIFT
    and    \end, \end, #PTRS_PER_TABLE - 1
    lsr    \phys, \phys, #SECTION_SHIFT
    mov    \tmp1, #\flags
    orr    \phys, \tmp1, \phys, lsl #SECTION_SHIFT
9999:    str    \phys, [\tbl, \start, lsl #3]
    add    \start, \start, #1
    add    \phys, \phys, #SECTION_SIZE
    cmp    \start, \end
    b.ls    9999b
.endm

.macro table_descriptor, tbl, virt, tmp1, tmp2
    table_entry \tbl, \virt, PGD_SHIFT, \tmp1, \tmp2
    table_entry \tbl, \virt, PUD_SHIFT, \tmp1, \tmp2
.endm

.global create_page_tables
create_page_tables:
    mov x29, x30

    adrp x0, pg_dir
    mov  x1, #KERNEL_VA_BEGIN
    table_descriptor x0, x1, x2, x3

    mov x1, xzr
    mov x2, #KERNEL_VA_BEGIN
    ldr x3, =(KERNEL_VA_BEGIN + DEVICE_BASE - SECTION_SIZE)
    block_entry x0, x1, x2, x3, MMU_FLAGS_MEMORY, x4

    mov x1, #DEVICE_BASE
    ldr x2, =(KERNEL_VA_BEGIN + DEVICE_BASE)
    ldr x3, =(KERNEL_VA_BEGIN + DEVICE_BASE - SECTION_SIZE)
    block_entry x0, x1, x2, x3, MMU_FLAGS_DEVICE, x4

    mov x30, x29
    ret