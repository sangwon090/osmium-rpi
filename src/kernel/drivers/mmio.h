#ifdef RPI4
    #define PERIPHERAL_BASE 0xFE000000
#else
    #define PERIPHERAL_BASE 0x3F000000
#endif

#ifndef MMIO_H
#define MMIO_H

inline unsigned int mmio_read(unsigned long reg)
{
    return *(volatile unsigned int*) reg;
}

inline void mmio_write(unsigned long reg, unsigned int value)
{
    *(volatile unsigned int*) reg = value;
}

#endif