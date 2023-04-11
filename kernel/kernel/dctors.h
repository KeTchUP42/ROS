#ifndef DCTORS_H
#define DCTORS_H

#include <stdint.h>

#include <libc/stdlib.h>

typedef void (*ctor_type)(void);
typedef void (*dtor_type)(void);

/**
 * The larger the PRIOR, the earlier the function is called in the corresponding phase.
 *
 * PRIOR is a standard linker-script way to sort sections:
 * https://www.sourceware.org/binutils/docs/ld.html#Input-Section-Wildcards
 *
 * In the kernel linker-script, SORT_BY_INIT_PRIORITY is used to sort all subsections.
 */

#define CTOR( FUNC, PRIOR ) ctor_type ctor##_##FUNC \
    SECTION(INIT_SECTION "." TO_STRING(PRIOR)) = &FUNC;

#define DTOR( FUNC, PRIOR ) dtor_type dtor##_##FUNC \
    SECTION(FINI_SECTION "." TO_STRING(PRIOR)) = &FUNC;

extern ctor_type __init_array_start[];
extern ctor_type __init_array_end[];
extern dtor_type __fini_array_start[];
extern dtor_type __fini_array_end[];

#endif //DCTORS_H
