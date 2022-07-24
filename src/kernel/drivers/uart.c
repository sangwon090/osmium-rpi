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

char uart_read()
{
    char c;

    while(!(mmio_read(AUX_MU_LSR_REG) & 0x01));
    c = (char) mmio_read(AUX_MU_IO_REG);

    return c;
}

void uart_write(char c)
{
    while(!(mmio_read(AUX_MU_LSR_REG) & 0x20));
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

void uart_hex(unsigned int hex)
{
    unsigned int n;
    int c;

    for(c=28; c>=0; c-=4) {
        n = (hex >> c) & 0xF;
        n += (n>9) ? 0x37 : 0x30;
        uart_write(n);
    }
}