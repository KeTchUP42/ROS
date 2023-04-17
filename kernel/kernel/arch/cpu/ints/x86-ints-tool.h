#ifndef CPU_INTS_X86_INTS_TOOL_H
#define CPU_INTS_X86_INTS_TOOL_H

#ifndef KERNEL_CODE
#error "DRIVERS ARE ONLY AVAILABLE IN THE KERNEL CODE!"
#endif

#ifndef ARCH_X86
#error "THIS CODE ONLY AVAILABLE ON X86 ARCHITECTURE!"
#endif

#include <libc/stdlib.h>

#define INT_NAME( NUM ) MACRO_CONCAT_2(__int_handler_, NUM)

#define I386_INT_DEF( NUM, FUNC )    \
void INT_NAME(NUM)(void);            \
__asm__(                             \
    ".intel_syntax noprefix      \n" \
TO_STRING(__int_handler_##NUM)": \n" \
    "call " #FUNC"               \n" \
    "iret                        \n" \
    ".att_syntax                 \n" \
);

#endif // CPU_INTS_X86_INTS_TOOL_H
