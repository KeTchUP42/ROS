#ifndef CPU_CLOCK_TIME_H
#define CPU_CLOCK_TIME_H

#ifdef ARCH_X86
#include "x86-time.h"
#else
#error "KERNEL CAN NOT BE BUILD WITH SUCH ARCHITECTURE!"
#endif

#endif // CPU_CLOCK_TIME_H
