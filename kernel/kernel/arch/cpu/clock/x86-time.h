#ifndef CPU_CLOCK_X86_TIME_H
#define CPU_CLOCK_X86_TIME_H

#ifndef KERNEL_CODE
#error "DRIVERS ARE ONLY AVAILABLE IN THE KERNEL CODE!"
#endif

#ifndef ARCH_X86
#error "THIS CODE ONLY AVAILABLE ON X86 ARCHITECTURE!"
#endif

#include <kernel/arch/io/ports/x86-ports.h>

#define WAIT_FOR_MILLISECOND (10000)

static inline void wait(void)
{
    outb(0x80, 0);
}

#endif // CPU_CLOCK_X86_TIME_H
