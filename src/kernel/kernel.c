#include "drivers/uart.h"
#include "drivers/mailbox.h"

void main()
{
    uart_init();
    uart_print("OSMIUM (");
    uart_print(__DATE__);
    uart_print(" ");
    uart_print(__TIME__);
    uart_print(")\n\n");

    mailbox[0] = 4 * 7; // buffer size
    mailbox[1] = 0; // code

    mailbox[2] = 0x00010002;    // tag id (get_revision)
    mailbox[3] = 4; // buffer size
    mailbox[4] = 0; // code
    mailbox[5] = 0; // buffer
    mailbox[6] = 0; // end tag

    mailbox_call();
    
    uart_print("rev: ");
    uart_hex(mailbox[5]);
    uart_print("\n");

    while(1)
    {
        char c = uart_read();

        if(c == '\r')
        {
            uart_write('\r');
            uart_write('\n');
        }
        else
        {
            uart_write(c);
        }
    }
}