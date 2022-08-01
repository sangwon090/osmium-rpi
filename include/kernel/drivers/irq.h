#include <types.h>
#include <drivers/mmio.h>

#ifndef IRQ_H
#define IRQ_H

#define IRQ_BASIC_PENDING       PERIPHERAL_BASE + 0x0000B200
#define IRQ_PENDING_1           PERIPHERAL_BASE + 0x0000B204
#define IRQ_PENDING_2           PERIPHERAL_BASE + 0x0000B208
#define FIQ_CONTROL             PERIPHERAL_BASE + 0x0000B20C
#define ENABLE_IRQS_1           PERIPHERAL_BASE + 0x0000B210
#define ENABLE_IRQS_2           PERIPHERAL_BASE + 0x0000B214
#define ENABLE_BASIC_IRQS       PERIPHERAL_BASE + 0x0000B218
#define DISABLE_IRQS_1          PERIPHERAL_BASE + 0x0000B21C
#define DISABLE_IRQS_2          PERIPHERAL_BASE + 0x0000B220
#define DISABLE_BASIC_IRQS      PERIPHERAL_BASE + 0x0000B224

#define IRQ_PENDING             PERIPHERAL_BASE + 0xB204
#define ENABLE_IRQS             PERIPHERAL_BASE + 0xB214

#define SYSTEM_TIMER_IRQ_0  (1 << 0)
#define SYSTEM_TIMER_IRQ_1  (1 << 1)
#define SYSTEM_TIMER_IRQ_2  (1 << 2)
#define SYSTEM_TIMER_IRQ_3  (1 << 3)

void irq_vector_init();
void irq_enable();
void irq_disable();

void enable_interrupt_controller();
void enable_interrupt(uint32_t irq);
void assign_target(uint32_t irq, uint32_t cpu);
void invalid_entry(int32_t type, uint64_t esr, uint64_t elr);
void handle_irq();

#endif