#include <drivers/aux.h>
#include <drivers/gpio.h>
#include <drivers/uart.h>

static int8_t *itoa_buffer[32];

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

int8_t uart_read()
{
    int8_t c;

    while(!(mmio_read(AUX_MU_LSR_REG) & 0x01));
    c = (int8_t) mmio_read(AUX_MU_IO_REG);

    return c;
}

void uart_write(int8_t c)
{
    while(!(mmio_read(AUX_MU_LSR_REG) & 0x20));
    mmio_write(AUX_MU_IO_REG, (uint32_t) c);
}

void uart_print(int8_t *str)
{
    while(*str)
    {
        uart_write(*str++);
    }
}

void uart_printf(int8_t *str, ...)
{
    va_list args;
    va_start(args, str);

    for(int i=0; str[i] != '\0'; i++)
    {
        if(str[i] == '%')
        {
            switch(str[i + 1])
            {
                case 'd':
                case 'i':
                case 'u':
                case 'o':
                case 'x':
                case 'X':
                    int32_t si = va_arg(args, int32_t), j = 0;
                    uint32_t ui = (uint32_t) si;

                    if(si == 0)
                    {
                        uart_write('0');
                        i ++;
                        break;
                    }

                    if(str[i+1] == 'd' || str[i+1] == 'i')
                    {
                        if(si < 0)
                        {
                            uart_write('-');
                            si *= -1;
                        }

                        while(si != 0)
                        {
                            itoa_buffer[j] = '0' + (si % 10);
                            si /= 10;
                            j ++;
                        }
                    }
                    else
                    {
                        if(str[i+1] == 'u')
                        {
                            while(ui != 0)
                            {
                                itoa_buffer[j] = '0' + (ui % 10);
                                ui /= 10;
                                j ++;
                            }
                        }
                        else if(str[i+1] == 'o')
                        {
                            while(ui != 0)
                            {
                                itoa_buffer[j] = '0' + (ui % 8);
                                ui /= 8;
                                j ++;
                            }
                        }
                        else if(str[i+1] == 'x')
                        {
                            while(ui != 0)
                            {
                                int x = (ui % 16);
                                itoa_buffer[j] = (x < 10) ? ('0' + x) : ('a' + (x - 10));
                                ui /= 16;
                                j ++;
                            }
                        }
                        else if(str[i+1] == 'X')
                        {
                            while(ui != 0)
                            {
                                int x = (ui % 16);
                                itoa_buffer[j] = (x < 10) ? ('0' + x) : ('A' + (x - 10));
                                ui /= 16;
                                j ++;
                            }
                        }
                    }

                    do
                    {
                        j --;
                        uart_write(itoa_buffer[j]);
                    } while(j >= 0);

                    i ++;
                    break;                
                case 'c':
                    int c = va_arg(args, int);
                    uart_write(c);
                    i ++;
                    break;

                case 's':
                    const char *s = va_arg(args, const char*);
                    for(int j=0; s[j] != '\0'; j++)
                        uart_write(s[j]);
                    i ++;
                    break;

                case '%':
                    uart_write('%');
                    i ++;
                    break;
            }
        }
        else
        {
            uart_write(str[i]);
        }
    }
}