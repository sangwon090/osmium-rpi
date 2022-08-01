#include <types.h>

#ifndef GIC_H
#define GIC_H

#define GIC_BASE    0xFF840000
#define GICD_BASE   (GIC_BASE + 0x1000) // GIC Distributor
#define GICC_BASE   (GIC_BASE + 0x2000) // GIC CPU Interfaces
#define GICH_BASE   (GIC_BASE + 0x4000) // GIC Virtual Interface Control Block
#define GICV_BASE   (GIC_BASE + 0x6000) // GIC Virtual CPU Interfaces

#define GICD_ISENABLER  (GICD_BASE + 0x100)
#define GICD_ITARGETSR  (GICD_BASE + 0x800)

#define GICC_IAR    (GICC_BASE + 0x000C)
#define GICC_EOIR   (GICC_BASE + 0x0010)

#define GIC_SYSTEM_TIMER_IRQ_0  0x60
#define GIC_SYSTEM_TIMER_IRQ_1  0x61
#define GIC_SYSTEM_TIMER_IRQ_2  0x62
#define GIC_SYSTEM_TIMER_IRQ_3  0x63

#endif