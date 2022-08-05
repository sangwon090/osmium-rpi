#include <drivers/gic.h>
#include <drivers/irq.h>
#include <drivers/mmio.h>
#include <drivers/timer.h>
#include <drivers/uart.h>

const char *error_messages[] = {
	"SYNC_INVALID_EL1t",
	"IRQ_INVALID_EL1t",		
	"FIQ_INVALID_EL1t",		
	"ERROR_INVALID_EL1T",		

	"SYNC_INVALID_EL1h",		
	"IRQ_INVALID_EL1h",		
	"FIQ_INVALID_EL1h",		
	"ERROR_INVALID_EL1h",		

	"SYNC_INVALID_EL0_64",		
	"IRQ_INVALID_EL0_64",		
	"FIQ_INVALID_EL0_64",		
	"ERROR_INVALID_EL0_64",	

	"SYNC_INVALID_EL0_32",		
	"IRQ_INVALID_EL0_32",		
	"FIQ_INVALID_EL0_32",		
	"ERROR_INVALID_EL0_32"	
};

void enable_interrupt(uint32_t irq)
{
    uint32_t n = irq / 32;
    uint32_t offset = irq % 32;
    uint32_t reg = GICD_ISENABLER + (n * 4);

    mmio_write(reg, 1 << offset);
}

void assign_target(uint32_t irq, uint32_t cpu)
{
    uint32_t n = irq / 4;
    uint32_t reg = GICD_ITARGETSR + (n * 4);

    mmio_write(reg, mmio_read(reg) | 1 << 8);
}

void enable_interrupt_controller()
{
    #ifdef RPI4
        assign_target(GIC_SYSTEM_TIMER_IRQ_1, 0);
        enable_interrupt(GIC_SYSTEM_TIMER_IRQ_1);
    #else
        mmio_write(ENABLE_IRQS_1, SYSTEM_TIMER_IRQ_1);
    #endif
}

void invalid_entry(int32_t type, uint64_t esr, uint64_t elr)
{
    uint64_t reg;
    asm volatile("mrs %0, elr_el1" : "=r" (reg));

    uart_printf("INVALID EXCEPTION\r\n");
    uart_printf("TYPE: %s\r\n", error_messages[type]);
    uart_printf("ESR: %X_%X\r\n", esr >> 32, esr);
    uart_printf("-  EC: %X\r\n", (esr >> 26) & 0b111111);
    uart_printf("-  IL: %X\r\n", (esr >> 25) & 0b1);
    uart_printf("- ISS: %X\r\n", esr & 0xFFFFFF);
    uart_printf("ELR: %X_%X\r\n", elr >> 32, elr);
}

void handle_irq()
{
    #ifdef RPI4
        uint32_t irq_ack = mmio_read(GICC_IAR);
        uint32_t irq = irq_ack & 0x2FF;
    #else
        uint32_t irq = mmio_read(IRQ_PENDING_1);
    #endif

    switch(irq)
    {
        #ifdef RPI4
        case GIC_SYSTEM_TIMER_IRQ_1:
            timer_handle_irq();
            mmio_write(GICC_EOIR, irq_ack);

            break;
        #else
        case SYSTEM_TIMER_IRQ_1:
            timer_handle_irq();
        
            break;
        #endif

        default:
            uart_printf("Unknown IRQ: %X\r\n", irq);
            break;
    }
}