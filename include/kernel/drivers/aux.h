#ifndef AUX_H
#define AUX_H

#include <drivers/mmio.h>

#define AUX_BASE    PERIPHERAL_BASE + 0x00215000

#define AUX_IRQ                 AUX_BASE + 0x00
#define AUX_ENABLES             AUX_BASE + 0x04
#define AUX_MU_IO_REG           AUX_BASE + 0x40
#define AUX_MU_IER_REG          AUX_BASE + 0x44
#define AUX_MU_IIR_REG          AUX_BASE + 0x48
#define AUX_MU_LCR_REG          AUX_BASE + 0x4C
#define AUX_MU_MCR_REG          AUX_BASE + 0x50
#define AUX_MU_LSR_REG          AUX_BASE + 0x54
#define AUX_MU_MSR_REG          AUX_BASE + 0x58
#define AUX_MU_SCRATCH          AUX_BASE + 0x5C
#define AUX_MU_CNTL_REG         AUX_BASE + 0x60
#define AUX_MU_STAT_REG         AUX_BASE + 0x64
#define AUX_MU_BAUD_REG         AUX_BASE + 0x68
#define AUX_SPI1_CNTL0_REG      AUX_BASE + 0x80
#define AUX_SPI1_CNTL1_REG      AUX_BASE + 0x84
#define AUX_SPI1_STAT_REG       AUX_BASE + 0x88
#define AUX_SPI1_PEEK_REG       AUX_BASE + 0x8C
#define AUX_SPI1_IO_REGa        AUX_BASE + 0xA0
#define AUX_SPI1_IO_REGb        AUX_BASE + 0xA4
#define AUX_SPI1_IO_REGc        AUX_BASE + 0xA8
#define AUX_SPI1_IO_REGd        AUX_BASE + 0xAC
#define AUX_SPI1_TXHOLD_REGa    AUX_BASE + 0xB0
#define AUX_SPI1_TXHOLD_REGb    AUX_BASE + 0xB4
#define AUX_SPI1_TXHOLD_REGc    AUX_BASE + 0xB8
#define AUX_SPI1_TXHOLD_REGd    AUX_BASE + 0xBC
#define AUX_SPI2_CNTL0_REG      AUX_BASE + 0xC0
#define AUX_SPI2_CNTL1_REG      AUX_BASE + 0xC4
#define AUX_SPI2_STAT_REG       AUX_BASE + 0xC8
#define AUX_SPI2_PEEK_REG       AUX_BASE + 0xCC
#define AUX_SPI2_IO_REGa        AUX_BASE + 0xE0
#define AUX_SPI2_IO_REGb        AUX_BASE + 0xE4
#define AUX_SPI2_IO_REGc        AUX_BASE + 0xE8
#define AUX_SPI2_IO_REGd        AUX_BASE + 0xEC
#define AUX_SPI2_TXHOLD_REGa    AUX_BASE + 0xF0
#define AUX_SPI2_TXHOLD_REGb    AUX_BASE + 0xF4
#define AUX_SPI2_TXHOLD_REGc    AUX_BASE + 0xF8
#define AUX_SPI2_TXHOLD_REGd    AUX_BASE + 0xFC

#endif