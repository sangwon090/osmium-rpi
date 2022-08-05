#include <drivers/mailbox.h>
#include <drivers/uart.h>

volatile uint32_t __attribute__((aligned(16))) mailbox[128];

uint32_t mailbox_call()
{
    uint32_t data = ((uint32_t)((uint64_t) &mailbox) & ~0xF) | 0x8;
    
    while(mmio_read(MAILBOX_STATUS) & MAILBOX_FULL);
    mmio_write(MAILBOX_WRITE, data);

    while(1)
    {
        while(mmio_read(MAILBOX_STATUS) & MAILBOX_EMPTY);
        if(mmio_read(MAILBOX_READ) == data) return mailbox[1] == MAILBOX_FULL;
    }

    return 0;
}