#include <drivers/uart.h>
#include <drivers/mailbox.h>
#include <drivers/framebuffer.h>
#include <drivers/irq.h>
#include <drivers/timer.h>
#include <drivers/mmu.h>
#include <mm.h>
#include <utils.h>
#include <types.h>

void task(int8_t *str)
{
    uint64_t begin = timer_get_jiffies();

    while(1)
    {
        uart_printf("[%s] allocated pages: %d\r\n", str, mm_get_count());    
        uart_printf("[%s] max pages: %d\r\n\r\n", str, PAGING_PAGES);

        for(int i=0; i<1000000; i++)
        {
            uint64_t end = timer_get_jiffies();

            if(end - begin > TIMER_HZ / 2)
            {
                break;
            }
        }
    }
}

void process(char *array)
{
    while (1){
        for (int i = 0; i < 5; i++){
            uart_printf(array[i]);
            delay(100000);
        }
    }
}

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

    int32_t p1 = process_create(&process, "p1");
    if(p1 != 0)
    {
        uart_printf("cannot launch process 1: %d\r\n", p1);
    }

    p1 = process_create(&process, "p2");
    if(p1 != 0)
    {
        uart_printf("cannot launch process 2: %d\r\n", p1);
    }

    while(1)
    {
        schedule();
    }

    // init mmu
    // mmu_init();

    while(1)
    {
        uart_write(uart_read());
    }
}