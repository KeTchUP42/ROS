#ifndef CPU_INTS_INTS_TOOL_H
#define CPU_INTS_INTS_TOOL_H

#ifdef ARCH_X86
#include "x86-ints-tool.h"
#include "x86-vector.h"
#else
#error "KERNEL CAN NOT BE BUILD WITH SUCH ARCHITECTURE!"
#endif

#endif // CPU_INTS_INTS_TOOL_H
