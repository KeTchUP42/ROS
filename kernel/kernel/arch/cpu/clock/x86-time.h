#ifndef CPU_CLOCK_X86_TIME_H
#define CPU_CLOCK_X86_TIME_H

/**********************************/
#include <kernel/arch/x86-driver.h>
/**********************************/

#include <kernel/arch/io/ports/x86-ports.h>

#define WAIT_FOR_MILLISECOND (10000)

static inline void cpu_wait(void)
{
    outb(0x80, 0);
}

#endif // CPU_CLOCK_X86_TIME_H
