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

    for(int y=0; y<1080; y++)
    {
        for(int x=0; x<1920; x++)
        {
            fb_set_pixel(x, y, COLOR(240, 240, 240, 0));
        }
    }

    // print execution level
    asm volatile ("mrs %0, CurrentEL" : "=r" (reg));
    uart_printf("Execution Level: EL%d\n", (reg >> 2) & 0b11);

    // init mmu
    mmu_init();

    while(1)
    {
        uint8_t c = uart_read();
        uart_write(c);

        if(c == '\r') uart_write('\n');
    }
}