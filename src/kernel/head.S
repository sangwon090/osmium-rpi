#include <cpu/mmu.h>

.section ".text.boot"

.global _start

_start:
    // if((mpdir_el1 & 0xFF)) > 0) hang(); else init();
    mrs x0, mpidr_el1   // x0 = mpdir_el1
    and x0, x0, #0xFF   // x0 &= 0xFF
    cbz x0, init        // if(x0 > 0) goto hang
    b hang              // else goto init

init:
    ldr x0, =_start     // x0 = &_start

    // if(CurrentEL & 0x0C == 0x0C) init_el2();
    mrs x1, CurrentEL   // x0 = CurrentEL
    and x1, x1, #0x0C   // x0 &= 0x0C
    cmp x1, #0x0C       // if x0 == EL3(0b1100) continue
    bne init_el2        // else goto init_el2

    // in case of EL3
    mov x2, #0x05B1     // x2 = 0x05B1
    msr scr_el3, x2     // scr_el3 = x2
    mov x2, #0x03C9     // x2 = 0x03C9
    msr spsr_el3, x2    // 
    adr x2, init_el2    //
    msr elr_el3, x2     //
    eret                // return;

init_el2:
    cmp x1, #4          // if x0 == EL2(0b0100) goto init_stack
    beq init_stack      // else continue
    msr sp_el1, x0

    // enable CNTP for EL1
    mrs     x1, cnthctl_el2
    orr     x1, x1, #3
    msr     cnthctl_el2, x1
    msr     cntvoff_el2, xzr
    // enable AArch64 in EL1
    mov     x1, #(1 << 31)      // AArch64
    orr     x1, x1, #(1 << 1)   // SWIO hardwired on Pi3
    msr     hcr_el2, x1
    mrs     x1, hcr_el2
    // Setup SCTLR access
    mov     x2, #0x0800
    movk    x2, #0x30d0, lsl #16
    msr     sctlr_el1, x2
    // change execution level to EL1
    mov     x2, #0x3c4
    msr     spsr_el2, x2
    adr     x2, init_stack
    msr     elr_el2, x2
    eret

init_stack:
    // memzero(__bss_start, __bss_end);
    mov sp, x0          // sp = x0
    adr x0, __bss_start // x0 = bss_begin
    adr x1, __bss_end   // x1 = bss_end
    sub x1, x1, x0      // x1 = x1 - x0
    bl  memzero         // call memzero

    // mmu_init();
    bl  mmu_init
    
    // enable mmu
    mov x0, #0xFFFF000000000000
    add sp, x0, SECTION_SIZE

    adrp x0, pg_dir
    msr  ttbr1_el1, x0

    ldr x0, =(MMU_TCR)
    msr tcr_el1, x0

    ldr x0, =(MMU_MAIR)
    msr mair_el1, x0

    mov x0, #1
    msr sctlr_el1, x0

    // kernel_main();
    bl  kernel_main

    // hang();
    b   hang

hang:
    wfe
    b hang

.global memzero
memzero:
	str xzr, [x0], #8
	subs x1, x1, #8
	b.gt memzero
	ret

/*
init_reg:
    // sctlr_el1: little endian, i-cache disabled, d-cache disabled, MMU disabled
    mov  x0, #0x0800
    movk x0, #0x30D0, lsl #16   // x0 = 0b00110000110100000000100000000000
    msr  sctlr_el1, x0          // sctlr_el1 = x0

    // hcr_el2: enable aarch64 in EL1
    mov  x0, #0x0000
    movk x0, #0x4000, lsl #16   // x0 = 0b01000000000000000000000000000000
    msr  hcr_el2, x0            // hcr_el1 = x0

    // scr_el3: non-secure, enable aarch64 in EL2
    mov  x0, #0x0219
    movk x0, #0x0000, lsl #16   // x0 = 0b00000000000000000000001000011001
    msr  scr_el3, x0            // scr_el3 = x0

    // spsr_el3: mask SError, IRQ and FIQ (EL1h)
    mov  x0, 0x00E5
    movk x0, 0x0000, lsl#16     // x0 = 0b00000000000000000000000011100101
    msr  spsr_el3, x0           // spsr_el3 = x0

    // elr_el3: jump to init_el1
    adr x0, init_el1    // x0 = &el1_init
    msr elr_el3, x0     // elr_el3 = x0

    eret
*/