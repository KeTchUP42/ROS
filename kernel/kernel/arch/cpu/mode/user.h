#ifndef CPU_MODE_USER_H
#define CPU_MODE_USER_H

#ifdef ARCH_X86
#include "x86-user.h"
#else
#error "KERNEL CAN NOT BE BUILD WITH SUCH ARCHITECTURE!"
#endif

#endif // CPU_MODE_USER_H
