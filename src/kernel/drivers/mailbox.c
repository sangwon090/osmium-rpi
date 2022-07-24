#include "mailbox.h"
#include "uart.h"

volatile unsigned int __attribute__((aligned(16))) mailbox[128];

unsigned int mailbox_call()
{
    unsigned int data = ((unsigned int)((unsigned long) &mailbox) & ~0xF) | 0x8;
    
    while(mmio_read(MAILBOX_STATUS) & MAILBOX_FULL);
    mmio_write(MAILBOX_WRITE, data);

    while(1)
    {
        while(mmio_read(MAILBOX_STATUS) & MAILBOX_EMPTY);
        if(mmio_read(MAILBOX_READ) == data) return mailbox[1] == MAILBOX_FULL;
    }

    return 0;
}