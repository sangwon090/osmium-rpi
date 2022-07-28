#include <drivers/uart.h>
#include <drivers/mailbox.h>
#include <mmu.h>

void main()
{
    uint64_t reg;

    uart_init();
    uart_printf("OSMIUM (%s %s)\n\n", __DATE__, __TIME__);
    uart_printf("%c.%c.%c.%c.%c.%c.%c.%c.%c\n%s\n", 'G', 'L', 'A', 'M', 'O', 'R', 'O', 'U', 'S', "First class~~~\n");
    uart_printf("%i %d %u %o %x %X\n", 0, -1, -1, 1024, 0xDEADC0DE, 0xDEADBEEF);

    // print execution level
    asm volatile ("mrs %0, CurrentEL" : "=r" (reg));
    uart_printf("Execution Level: EL%d\n", reg >> 2);

    // init mmu
    mmu_init();

    while(1)
    {
        uint8_t c = uart_read();
        uart_write(c);

        if(c == '\r') uart_write('\n');
    }
}