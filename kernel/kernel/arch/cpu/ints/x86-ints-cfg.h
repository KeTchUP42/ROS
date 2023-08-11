#ifndef CPU_INTS_X86_INTS_TOOL_H
#define CPU_INTS_X86_INTS_TOOL_H

/**********************************/
#include <kernel/arch/x86-driver.h>
/**********************************/

#include <stdint.h>
#include <libc/stdlib.h>

#include "x86-ihard.h"

typedef void (*cpu_int_callback_t)(uint32_t);

#define INT_NAME( NUM ) MACRO_CONCAT_2(__int_handler_, NUM)

#define CPU_INT_DEF( NUM, FUNC )     \
void INT_NAME(NUM)(void);            \
__asm__(                             \
    ".intel_syntax noprefix      \n" \
TO_STRING(__int_handler_##NUM)": \n" \
    "push OFFSET FLAT:" #FUNC "  \n" \
    "push " #NUM "               \n" \
    "call __int_handler_wrapper  \n" \
    "add esp, 8                  \n" \
    "iret                        \n" \
    ".att_syntax                 \n" \
);

#endif // CPU_INTS_X86_INTS_TOOL_H
