#include <drivers/mmio.h>
#include <drivers/uart.h>
#include <drivers/irq.h>
#include <drivers/timer.h>

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

void enable_interrupt_controller()
{
    mmio_write(ENABLE_IRQS_1, SYSTEM_TIMER_IRQ_1);
}

void invalid_entry(int32_t type, uint64_t esr, uint64_t elr)
{
    uint64_t reg;
    asm volatile("mrs %0, elr_el1" : "=r" (reg));

    uart_printf("INVALID EXCEPTION\n");
    uart_printf("TYPE: %s\n", error_messages[type]);
    uart_printf(" ESR: %X_%X\n", esr >> 32, esr);
    uart_printf(" ELR: %X_%X\n", elr >> 32, elr);
    uart_printf("  EC: %X\n", (esr >> 26) & 0b111111);
    uart_printf("  IL: %X\n", (esr >> 25) & 0b1);
    uart_printf(" ISS: %X\n", esr & 0xFFFFFF);
}

void handle_irq()
{
    uint32_t irq = mmio_read(IRQ_PENDING_1);

    switch(irq)
    {
        case SYSTEM_TIMER_IRQ_1:
            handle_timer_irq();
            break;
        default:
            uart_printf("Unknown IRQ: %X\n", irq);
            break;
    }
}