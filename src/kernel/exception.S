#include <exception.h>

.macro    ventry    label
    .balign 0x80
    b \label
.endm

.macro pusha
    sub	sp, sp, #STACK_FRAME_SIZE
    stp	x0, x1, [sp, #16 * 0]
    stp	x2, x3, [sp, #16 * 1]
    stp	x4, x5, [sp, #16 * 2]
    stp	x6, x7, [sp, #16 * 3]
    stp	x8, x9, [sp, #16 * 4]
    stp	x10, x11, [sp, #16 * 5]
    stp	x12, x13, [sp, #16 * 6]
    stp	x14, x15, [sp, #16 * 7]
    stp	x16, x17, [sp, #16 * 8]
    stp	x18, x19, [sp, #16 * 9]
    stp	x20, x21, [sp, #16 * 10]
    stp	x22, x23, [sp, #16 * 11]
    stp	x24, x25, [sp, #16 * 12]
    stp	x26, x27, [sp, #16 * 13]
    stp	x28, x29, [sp, #16 * 14]

    mrs x22, elr_el1
    mrs x23, spsr_el1
    
    str	x30, [sp, #16 * 15] 
    str x23, [sp, #16 * 16]
.endm

.macro popa
    ldr x23, [sp, #16 * 16]
    ldp x30, x22, [sp, #16 * 15]

    msr elr_el1, x22
    msr spsr_el1, x23

    ldp	x0, x1, [sp, #16 * 0]
    ldp	x2, x3, [sp, #16 * 1]
    ldp	x4, x5, [sp, #16 * 2]
    ldp	x6, x7, [sp, #16 * 3]
    ldp	x8, x9, [sp, #16 * 4]
    ldp	x10, x11, [sp, #16 * 5]
    ldp	x12, x13, [sp, #16 * 6]
    ldp	x14, x15, [sp, #16 * 7]
    ldp	x16, x17, [sp, #16 * 8]
    ldp	x18, x19, [sp, #16 * 9]
    ldp	x20, x21, [sp, #16 * 10]
    ldp	x22, x23, [sp, #16 * 11]
    ldp	x24, x25, [sp, #16 * 12]
    ldp	x26, x27, [sp, #16 * 13]
    ldp	x28, x29, [sp, #16 * 14]
    add	sp, sp, #STACK_FRAME_SIZE		
    eret
.endm

.macro handle_invalid_entry type
    pusha
    mov	x0, #\type
    mrs	x1, esr_el1
    mrs	x2, elr_el1
    bl invalid_entry
    b err_hang
.endm

.balign 0x800
.global vectors
vectors:
    ventry sync_invalid_el1t
    ventry el1_irq
    ventry fiq_invalid_el1t
    ventry error_invalid_el1t

    ventry sync_invalid_el1h
    ventry irq_invalid_el1h
    ventry fiq_invalid_el1h
    ventry error_invalid_el1h

    ventry sync_invalid_el0_64
    ventry irq_invalid_el0_64
    ventry fiq_invalid_el0_64
    ventry error_invalid_el0_64

    ventry sync_invalid_el0_32
    ventry irq_invalid_el0_32
    ventry fiq_invalid_el0_32
    ventry error_invalid_el0_32

sync_invalid_el1t:
    handle_invalid_entry SYNC_INVALID_EL1t

irq_invalid_el1t:
    handle_invalid_entry IRQ_INVALID_EL1t

fiq_invalid_el1t:
    handle_invalid_entry FIQ_INVALID_EL1t

error_invalid_el1t:
    handle_invalid_entry ERROR_INVALID_EL1t

sync_invalid_el1h:
    handle_invalid_entry SYNC_INVALID_EL1h

irq_invalid_el1h:
    handle_invalid_entry IRQ_INVALID_EL1h

fiq_invalid_el1h:
    handle_invalid_entry FIQ_INVALID_EL1h

error_invalid_el1h:
    handle_invalid_entry ERROR_INVALID_EL1h

sync_invalid_el0_64:
    handle_invalid_entry SYNC_INVALID_EL0_64

irq_invalid_el0_64:
    handle_invalid_entry IRQ_INVALID_EL0_64

fiq_invalid_el0_64:
    handle_invalid_entry FIQ_INVALID_EL0_64

error_invalid_el0_64:
    handle_invalid_entry ERROR_INVALID_EL0_64

sync_invalid_el0_32:
    handle_invalid_entry SYNC_INVALID_EL0_32

irq_invalid_el0_32:
    handle_invalid_entry IRQ_INVALID_EL0_32

fiq_invalid_el0_32:
    handle_invalid_entry FIQ_INVALID_EL0_32

error_invalid_el0_32:
    handle_invalid_entry ERROR_INVALID_EL0_32

el1_irq:
    pusha 
    bl handle_irq
    popa

.global err_hang
err_hang:
    b err_hang