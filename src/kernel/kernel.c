#include <drivers/uart.h>
#include <drivers/mailbox.h>
#include <drivers/framebuffer.h>
#include <drivers/irq.h>
#include <drivers/timer.h>
#include <mmu.h>

void main()
{
    uart_init();
    uart_printf("OSMIUM (%s %s)\n\n", __DATE__, __TIME__);

    uart_printf("Initializing framebuffer... ");
    fb_init(640, 360);
    fb_draw_rect(0, 0, 640, 360, 0x00FFFFFF);
    uart_printf("DONE\n");

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