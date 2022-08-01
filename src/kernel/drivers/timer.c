#include <drivers/mmio.h>
#include <drivers/uart.h>
#include <drivers/timer.h>

static uint64_t jiffies, counter;

void timer_init()
{
    jiffies = 0;
    counter = mmio_read(TIMER_CLO);

    mmio_write(TIMER_C1, counter + TIMER_INTERVAL);
}

void timer_handle_irq()
{
    jiffies ++;
    counter = mmio_read(TIMER_CLO);
    mmio_write(TIMER_C1, counter + TIMER_INTERVAL);
    mmio_write(TIMER_CS, TIMER_CS_M1);

    if(jiffies % 250 == 0)
    {
        uart_printf("\r\ntick %ds", jiffies / 250);
    }
    else if(jiffies % 25 == 0)
    {
        uart_printf(".");
    }
}

uint64_t timer_get_jiffies()
{
    return jiffies;
}