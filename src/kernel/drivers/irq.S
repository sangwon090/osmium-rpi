.global irq_vector_init
irq_vector_init:
    adr x0, vectors
    msr VBAR_EL1, x0
    ret

.global irq_enable
irq_enable:
    msr DAIFCLR, #2
    ret

.global irq_disable
irq_disable:
    msr DAIFSET, #2
    ret