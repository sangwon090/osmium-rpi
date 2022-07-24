#include "aux.h"
#include "gpio.h"
#include "uart.h"

void uart_init()
{
    mmio_write(AUX_ENABLES,     0x01);
    mmio_write(AUX_MU_IER_REG,  0x00);
    mmio_write(AUX_MU_CNTL_REG, 0x00);
    mmio_write(AUX_MU_LCR_REG,  0x03);
    mmio_write(AUX_MU_MCR_REG,  0x00);
    mmio_write(AUX_MU_IER_REG,  0x00);
    mmio_write(AUX_MU_IIR_REG,  0xC6);
    mmio_write(AUX_MU_BAUD_REG, 0x021E);
    gpio_pull(14, 0);
    gpio_function(14, GPIO_FUNCTION_ALT5);
    gpio_pull(15, 0);
    gpio_function(15, GPIO_FUNCTION_ALT5);
    mmio_write(AUX_MU_CNTL_REG, 3);
}

unsigned int uart_is_ready()
{
    return mmio_read(AUX_MU_LSR_REG) & 0x20;
}

void uart_write(char c)
{
    while(!uart_is_ready());
    mmio_write(AUX_MU_IO_REG, (unsigned int) c);
}

void uart_print(char *str)
{
    while(*str)
    {
        if(*str == '\n') uart_write('\r');
        uart_write(*str++);
    }
}