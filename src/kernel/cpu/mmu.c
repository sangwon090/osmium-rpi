#include <cpu/mmu.h>

// TTBR    PGD     PUD     PMD
// PGD --> PUD --> PMD --> PTE

void mmu_init()
{
    uart_printf("mmu_init();");
    memzero(pg_dir, PG_DIR_SIZE);
    
    uint64_t table_addr = &pg_dir;
    uint64_t table_next_addr = table_addr + PAGE_SIZE;
    uint64_t virt_addr = 0xFFFF000000000000;

    mmu_create_table_entry(table_addr, table_next_addr, VA_START, PGD_SHIFT, MMU_FLAGS_PTABLE);
    table_addr += PAGE_SIZE;
    table_next_addr += PAGE_SIZE;

    mmu_create_block_entry(table_addr, 0, VA_START, VA_START + PHYS_MEMORY_SIZE, MMU_FLAGS_MEMORY);
    table_addr += PAGE_SIZE;
    table_next_addr += PAGE_SIZE;
    
    mmu_create_block_entry(table_addr, PERIPHERAL_BASE, VA_START + PERIPHERAL_BASE, VA_START + PERIPHERAL_BASE + PHYS_MEMORY_SIZE, MMU_FLAGS_DEVICE);
    table_addr += PAGE_SIZE;
    table_next_addr += PAGE_SIZE;
}

void mmu_create_table_entry(uint64_t table_addr, uint64_t table_next_addr, uint64_t virt_addr, uint64_t shift, uint64_t flags)
{
    uint64_t t1 = virt_addr >> shift;
    t1 &= (PTRS_PER_TABLE - 1);

    uint64_t t2 = table_next_addr | flags;
    *((uint64_t*)(table_addr + (t1 << 3))) = t2;
}

void mmu_create_block_entry(uint64_t table_addr, uint64_t phys_addr, uint64_t begin, uint64_t end, uint64_t flags)
{
    begin >>= SECTION_SHIFT;
    begin &= (PTRS_PER_TABLE - 1);

    end >>= SECTION_SHIFT;
    end &= (PTRS_PER_TABLE - 1);

    phys_addr >>= SECTION_SHIFT;
    phys_addr <<= SECTION_SHIFT;
    phys_addr  |= flags;

    do
    {
        *((uint64_t*)(table_addr + (begin >> 3))) = phys_addr;

        phys_addr += SECTION_SIZE;
        begin ++;
    } while(begin <= end);
}
