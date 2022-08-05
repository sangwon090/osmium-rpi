#include <drivers/mmio.h>
#include <drivers/timer.h>
#include <drivers/uart.h>
#include <task/sched.h>

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
    scheduler_tick();
}

uint64_t timer_get_jiffies()
{
    return jiffies;
}