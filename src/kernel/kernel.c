#include "drivers/uart.h"
//#include "drivers/io.h"
void main()
{
    uart_init();
    uart_print("Hello, World!\nfrom OSMIUM");

    //uart_init();
    //uart_writeText("Hello world from OSMIUM!");
        while (1);
}