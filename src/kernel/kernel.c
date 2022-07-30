#include <drivers/uart.h>
#include <drivers/mailbox.h>
#include <drivers/framebuffer.h>
#include <mmu.h>

void main()
{
    uint64_t reg;

    uart_init();
    uart_printf("OSMIUM (%s %s)\n\n", __DATE__, __TIME__);

    uart_printf("Initializing framebuffer... ");
    if(fb_init(1920, 1080))
    {
        uart_printf("DONE\n");
    }
    else
    {
        uart_printf("ERROR\n");
    }

    // init exception vector
    uart_printf("Initializing exception... ");
    irq_vector_init();
    timer_init();
    enable_interrupt_controller();
    irq_enable();
    uart_printf("DONE\n");

    // init mmu
    mmu_init();

    while(1)
    {
        uart_write(uart_read());
    }
}