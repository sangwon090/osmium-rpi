#include <mm.h>
#include <types.h>
#include <utils.h>
#include <drivers/fb.h>
#include <drivers/irq.h>
#include <drivers/mailbox.h>
#include <drivers/timer.h>
#include <drivers/uart.h>

void kernel_main()
{    
    uart_init();
    uart_printf("OSMIUM (%s %s)\r\n\r\n", __DATE__, __TIME__);

    uart_printf("Initializing framebuffer... ");
    fb_init(640, 360);
    fb_draw_rect(0, 0, 640, 360, 0x00FFFFFF);
    uart_printf("DONE\r\n");

    // init exception vector
    uart_printf("Initializing exception... ");
    irq_vector_init();
    timer_init();
    enable_interrupt_controller();
    irq_enable();
    uart_printf("DONE\r\n");

    while(1)
    {
        schedule();
    }

    while(1)
    {
        uart_write(uart_read());
    }
}