#ifndef CPU_INTS_X86_VECTOR_H
#define CPU_INTS_X86_VECTOR_H

#ifndef KERNEL_CODE
#error "DRIVERS ARE ONLY AVAILABLE IN THE KERNEL CODE!"
#endif

#ifndef ARCH_X86
#error "THIS CODE ONLY AVAILABLE ON X86 ARCHITECTURE!"
#endif

#include "x86-ints-tool.h"

#define CPU_INTS_VECTOR_SIZE (256)
#define DEFINE_HANDLER( NUM ) [NUM] = &INT_NAME(NUM),
#define GET_INT_HANDLER( NUM ) ISR_TABLE[NUM]

#define INT_VECTOR_START void *ISR_TABLE[CPU_INTS_VECTOR_SIZE] = {
#define INT_VECTOR_END };

/**
 * @brief Global table with addresses of int handlers.
 */
extern void *ISR_TABLE[CPU_INTS_VECTOR_SIZE];

#endif // CPU_INTS_X86_VECTOR_H
