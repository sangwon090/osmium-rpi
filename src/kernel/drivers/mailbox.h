#include "mmio.h"

#define MAILBOX_BASE    PERIPHERAL_BASE + 0x0000B880

#define MAILBOX_READ    MAILBOX_BASE + 0x00
#define MAILBOX_PEEK    MAILBOX_BASE + 0x10
#define MAILBOX_SENDER  MAILBOX_BASE + 0x14
#define MAILBOX_STATUS  MAILBOX_BASE + 0x18
#define MAILBOX_CONFIG  MAILBOX_BASE + 0x1C
#define MAILBOX_WRITE   MAILBOX_BASE + 0x20

#define MAILBOX_FULL    0x80000000
#define MAILBOX_EMPTY   0x40000000

#ifndef MAILBOX_H
#define MAILBOX_H

extern volatile unsigned int mailbox[128];

unsigned int mailbox_call();

#endif