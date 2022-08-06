#ifndef MMIO_H
#define MMIO_H

#include <types.h>

#ifdef RPI4
    #define PERIPHERAL_BASE 0xFE000000
#else
    #define PERIPHERAL_BASE 0x3F000000
#endif

#ifndef __ASSEMBLER__

inline uint32_t mmio_read(uint64_t reg)
{
    return *(volatile uint32_t*) reg;
}

inline void mmio_write(uint64_t reg, uint32_t value)
{
    *(volatile uint32_t*) reg = value;
}

#endif

#endif