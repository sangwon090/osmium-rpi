#include <drivers/mmio.h>
#include <mmu.h>

#define PAGE_SIZE 4096

extern volatile uint8_t _data;
extern volatile uint8_t _end;

#define PAGESIZE    4096

// granularity
#define PT_PAGE     0b11        // 4k granule
#define PT_BLOCK    0b01        // 2M granule
// accessibility
#define PT_KERNEL   (0<<6)      // privileged, supervisor EL1 access only
#define PT_USER     (1<<6)      // unprivileged, EL0 access allowed
#define PT_RW       (0<<7)      // read-write
#define PT_RO       (1<<7)      // read-only
#define PT_AF       (1<<10)     // accessed flag
#define PT_NX       (1UL<<54)   // no execute
// shareability
#define PT_OSH      (2<<8)      // outter shareable
#define PT_ISH      (3<<8)      // inner shareable
// defined in MAIR register
#define PT_MEM      (0<<2)      // normal memory
#define PT_DEV      (1<<2)      // device MMIO
#define PT_NC       (2<<2)      // non-cachable

void mmu_init()
{
    uint64_t reg, pa_range;
    uint64_t *ptable = (unsigned long*) &_end;
    unsigned long b, data_page = (unsigned long)&_data/PAGESIZE;

    // TTBR0
    ptable[0] = (unsigned long)((unsigned char*) &_end + PAGE_SIZE * 2);
                
    ptable[0]=(unsigned long)((unsigned char*)&_end+2*PAGESIZE) |    // physical address
        PT_PAGE |     // it has the "Present" flag, which must be set, and we have area in it mapped by pages
        PT_AF |       // accessed flag. Without this we're going to have a Data Abort exception
        PT_USER |     // non-privileged
        PT_ISH |      // inner shareable
        PT_MEM;       // normal memory

    // identity L2, first 2M block
    ptable[2*512]=(unsigned long)((unsigned char*)&_end+3*PAGESIZE) | // physical address
        PT_PAGE |     // we have area in it mapped by pages
        PT_AF |       // accessed flag
        PT_USER |     // non-privileged
        PT_ISH |      // inner shareable
        PT_MEM;       // normal memory

    // identity L2 2M blocks
    b=PERIPHERAL_BASE>>21;
    // skip 0th, as we're about to map it by L3
    for(uint64_t r=1;r<512;r++)
        ptable[2*512+r]=(unsigned long)((r<<21)) |  // physical address
        PT_BLOCK |    // map 2M block
        PT_AF |       // accessed flag
        PT_NX |       // no execute
        PT_USER |     // non-privileged
        (r>=b? PT_OSH|PT_DEV : PT_ISH|PT_MEM); // different attributes for device memory

    // identity L3
    for(uint64_t r=0;r<512;r++)
        ptable[3*512+r]=(unsigned long)(r*PAGESIZE) |   // physical address
        PT_PAGE |     // map 4k
        PT_AF |       // accessed flag
        PT_USER |     // non-privileged
        PT_ISH |      // inner shareable
        ((r<0x80||r>=data_page)? PT_RW|PT_NX : PT_RO); // different for code and data

    // TTBR1, kernel L1
    ptable[512+511]=(unsigned long)((unsigned char*)&_end+4*PAGESIZE) | // physical address
        PT_PAGE |     // we have area in it mapped by pages
        PT_AF |       // accessed flag
        PT_KERNEL |   // privileged
        PT_ISH |      // inner shareable
        PT_MEM;       // normal memory

    // kernel L2
    ptable[4*512+511]=(unsigned long)((unsigned char*)&_end+5*PAGESIZE) |   // physical address
        PT_PAGE |     // we have area in it mapped by pages
        PT_AF |       // accessed flag
        PT_KERNEL |   // privileged
        PT_ISH |      // inner shareable
        PT_MEM;       // normal memory

    // kernel L3
    ptable[5*512]=(unsigned long)(PERIPHERAL_BASE+0x00201000) |   // physical address
        PT_PAGE |     // map 4k
        PT_AF |       // accessed flag
        PT_NX |       // no execute
        PT_KERNEL |   // privileged
        PT_OSH |      // outter shareable
        PT_DEV;       // device memory

    // get ID_AA64MMFR0_EL1
    reg = 0;
    asm volatile("mrs %0, ID_AA64MMFR0_EL1" : "=r" (reg));
    pa_range = reg & 0b1111;

    // set MAIR_EL1
    reg  = 0;
    reg |= (0b01000100 << 16);  // non cacheable
    reg |= (0b00000100 << 8);   // device, nGnRE
    reg |= (0b11111111);        // normal, IWBWA, OWBWA, NTR
    asm volatile("msr MAIR_EL1, %0" : : "r" (reg));

    // set TCR_EL1
    reg  = 0;
    reg |= (0b0 << 38);         // TBI1: top byte used in the address calculation
    reg |= (0b0 << 37);         // TBI0: top byte used in the address calculation
    reg |= (pa_range << 32);    // IPS size: physical address range supported

    reg |= (0b10 << 30);        // TG1: 4KB
    reg |= (0b11 << 28);        // SH1: inner shareable
    reg |= (0b01 << 26);        // ORGN1: normal memory, outer write-back read-allocate write-allocate cacheable
    reg |= (0b01 << 24);        // IRGN1: normal memory, outer write-back read-allocate write-allocate cacheable
    reg |= (0b011001 << 16);    // T1SZ: 512GB

    reg |= (0b00 << 14);        // TG0: 4KB
    reg |= (0b11 << 12);        // SH0: inner shareable
    reg |= (0b01 << 10);        // ORGN0: normal memory, outer write-back read-allocate write-allocate cacheable
    reg |= (0b01 << 8);         // IRGN0: normal memory, outer write-back read-allocate write-allocate cacheable
    reg |= (0b011001);          // T0SZ: 512GB

    asm volatile("msr TCR_EL1, %0" : : "r" (reg));
    asm volatile("isb");

    // set TTBR
    asm volatile("msr TTBR0_EL1, %0" : : "r" ((unsigned long) &_end + 1));
    asm volatile("msr TTBR1_EL1, %0" : : "r" ((unsigned long) &_end + 4097));

    // set SCTLR
    asm volatile("dsb ish");
    asm volatile("isb");
    asm volatile("mrs %0, SCTLR_EL1" : "=r" (reg));

    reg |= 0xC00800;
    reg &= ~((0b1<<25) |  // clear EE, little endian translation tables
            (0b1<<24)  |  // clear E0E
            (0b1<<19)  |  // clear WXN
            (0b1<<12)  |  // clear I, no instruction cache
            (0b1<<4)   |  // clear SA0
            (0b1<<3)   |  // clear SA
            (0b1<<2)   |  // clear C, no cache at all
            (0b1<<1));    // clear A, no aligment check
    reg |= (0b1);

    asm volatile("msr SCTLR_EL1, %0" : : "r" (reg));
    asm volatile("isb");
}