#ifndef DCTORS_H
#define DCTORS_H

#include <stdint.h>

#include <kernel/utils/kldinc.h>

typedef void (*ctor_type)(void);
typedef void (*dtor_type)(void);

#define CTOR( FUNC ) ctor_type ctor##_##FUNC SECTION(INIT_SECTION) = &FUNC;
#define DTOR( FUNC ) dtor_type dtor##_##FUNC SECTION(FINI_SECTION) = &FUNC;

extern uintptr_t __init_array_start[];
extern uintptr_t __init_array_end[];
extern uintptr_t __fini_array_start[];
extern uintptr_t __fini_array_end[];

#endif //DCTORS_H
