#include <drivers/mmio.h>
#include <drivers/timer.h>

unsigned int curVal = 0;

void timer_init()
{
    curVal = mmio_read(TIMER_CLO);
    curVal += TIMER_INTERVAL;
    mmio_write(TIMER_C1, curVal);
}

void handle_timer_irq()
{
    curVal += TIMER_INTERVAL;
    mmio_write(TIMER_C1, curVal);
    mmio_write(TIMER_CS, TIMER_CS_M1);
    uart_printf("Timer interrupt received\n");
}